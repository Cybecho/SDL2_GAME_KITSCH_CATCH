#include "GameLogic.h"

std::vector<std::unique_ptr<Mahjong>> g_vector; // 마작 블록 생성 벡터
std::vector<std::unique_ptr<Mahjong>> g_stack; // 마작 블록 스택 벡터
std::vector<bonk> g_bonks; // 효과 블록 생성 벡터

Mix_Chunk* wave1_;
Mix_Music* music1_;

SDL_Texture* text_score_;
SDL_Rect text_score_rect_;
TTF_Font* game_font_;

SDL_Rect g_bg_source_rect;
SDL_Rect g_bg_destination_rect;

void InitGame() {
    g_flag_running = true;

    g_bg_source_rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    g_bg_destination_rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    //! 최초 마작 블록 로드
    //! 아래 변수는 함수의 매개변수 이해를 돕기 위한 변수입니다
    int level = 0;
    int seed = 0;
    int numDims = 2;
    LoadMahjongBlocksFromCSV(level, seed, numDims);
}

void HandleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        //! 스페이스바 누르면 벡터&스택 초기화
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            g_vector.clear();
            g_stack.clear();
        }

        if (event.type == SDL_MOUSEMOTION) {
            int mouse_x = event.motion.x;
            int mouse_y = event.motion.y;

            for (auto& block : g_vector) {
                if (block->isClickable()) {
                    block->setHovered(block->isHovered(mouse_x, mouse_y));
                }
            }
        }

        //! 마우스 클릭 이벤트 처리
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouse_x = event.button.x;
            int mouse_y = event.button.y;

            for (auto it = g_vector.begin(); it != g_vector.end(); ++it) {
                //! clickEnable이 true인 경우에만 클릭 이벤트 처리
                if ((*it)->isClickable() && (*it)->isClicked(mouse_x, mouse_y)) {
                    (*it)->Play2Sound();

                    vector2stack(it); //! 클릭된 블록을 g_stack으로 이동
                    createBonk(mouse_x, mouse_y); //! 클릭 위치에 bonk 객체 생성
                    break;
                }
            }
        }
    }
}

void Update() {
    LoadMahjongBlocksIfEmpty();
    RemoveSameTypeBlocks();
    AlignStackBlocks();
    UpdateVectorBlocks();
    UpdateStackBlocks();
    UpdateBonks();
}

void Render() {
    SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255); // 배경색 설정
    SDL_RenderClear(g_renderer);

    // 블록 객체 렌더링
    for (const auto& block : g_vector) {
        block->render(g_renderer);
    }
    for (const auto& block : g_stack) {
        block->render(g_renderer);
    }

    // bonk 객체 렌더링
    for (const auto& bonk : g_bonks) {
        bonk.render(g_renderer);
    }

    SDL_RenderPresent(g_renderer);
}

//! CSV 파일에서 마작 블록을 읽어와서 g_vector에 생성
void LoadMahjongBlocksFromCSV(int level, int seed, int numDims) {
    g_vector.clear();

    for (int dim = 0; dim <= numDims; ++dim) {
        std::string filename = "../Resources/level/" + std::to_string(level) + "/" + std::to_string(seed) + "-" + std::to_string(dim) + ".csv";
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            continue;
        }
        std::string line;
        int row = 0;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string cell;
            int col = 0;

            while (std::getline(iss, cell, ',')) {
                int data = std::stoi(cell);

                int x = col * BLOCK_SIZE + PIVOT_X + (dim * BLOCK_SIZE / 2);
                int y = row * BLOCK_SIZE + PIVOT_Y + (dim * BLOCK_SIZE / 2);

                if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) {
                    switch (data) {
                    case 1:
                        g_vector.emplace_back(std::make_unique<Mahjong_A>(x, y, g_renderer));
                        break;
                    case 2:
                        g_vector.emplace_back(std::make_unique<Mahjong_B>(x, y, g_renderer));
                        break;
                    case 3:
                        g_vector.emplace_back(std::make_unique<Mahjong_C>(x, y, g_renderer));
                        break;
                    case 4:
                        g_vector.emplace_back(std::make_unique<Mahjong_D>(x, y, g_renderer));
                        break;
                    default:
                        break;
                    }
                }

                ++col;
            }

            ++row;
        }
    }
}

//! 클릭된 블록을 g_vector에서 g_stack으로 이동
void vector2stack(std::vector<std::unique_ptr<Mahjong>>::iterator it) {
    auto block = std::move(*it);
    g_vector.erase(it);

    // 객체의 위치를 g_stack의 크기에 따라 동적으로 계산합니다.
    int x = g_stack.size() * (BLOCK_SIZE / 2) + PIVOT_X;
    int y = BLOCK_SIZE + PIVOT_Y - (BLOCK_SIZE * 2);

    block->setX(x);
    block->setY(y);

    g_stack.emplace_back(std::move(block));

    // g_blocks와 g_stack의 사이즈 출력
    std::cout << "g_blocks size: " << g_vector.size() << "| g_stack size: " << g_stack.size() << std::endl;
}

//! 클릭된 위치에 bonk 객체 생성
void createBonk(int x, int y) {
    if (BLOCK_SCALE == 1) {
        g_bonks.emplace_back(x - (BLOCK_SIZE / 2), y - (BLOCK_SIZE / 2), g_renderer);
    }
    else {
        g_bonks.emplace_back(x - (BLOCK_SIZE / (BLOCK_SCALE * BLOCK_SCALE)), y - (BLOCK_SIZE / (BLOCK_SCALE * BLOCK_SCALE)), g_renderer);
    }
}

//! update 함수 : 벡터 영역이 비었을 때 csv 파일을 읽어와서 다시 로드
void LoadMahjongBlocksIfEmpty() {
    if (g_vector.empty()) {
        int level = 1;
        int seed = 0;
        int numDims = 3;
        LoadMahjongBlocksFromCSV(level, seed, numDims);
    }
}

//! update 함수 :  g_stack에서 같은 객체가 3개 이상 존재할 경우 pop 처리
void RemoveSameTypeBlocks() {
    map<string, int> typeCount;
    for (const auto& block : g_stack) {
        typeCount[block->getType()]++;
    }

    for (auto it = g_stack.begin(); it != g_stack.end();) {
        int localScore = 0;
        if (typeCount[(*it)->getType()] >= 3) {
            typeCount[(*it)->getType()] -= 3;

            // 제거되는 블록의 위치에서 createBonk() 호출
            for (int i = 0; i < 3; ++i) {
                int x = (it + i)->get()->getX() + BLOCK_SIZE / 2;
                int y = (it + i)->get()->getY() + BLOCK_SIZE / 2;
                createBonk(x, y);
            }

            it = g_stack.erase(it, it + 3);

            // g_stack에 있는 모든 Mahjong 객체의 shakeBlock() 호출 (입력 프레임에 따라 흔들림)
            for (auto& block : g_stack) {
                block->shakeBlocks(10);
            }
        }
        else {
            ++it;
        }
    }
}

//! update 함수 :  g_stack에 남은 객체들 순서대로 위치 조정
void AlignStackBlocks() {
    sortPairedBlocks(); //~ 쌍이 2개 이상인 객체들만 정렬

    for (size_t i = 0; i < g_stack.size(); ++i) {
        int x = i * (BLOCK_SIZE / 2 + 15) + PIVOT_X;
        int y = BLOCK_SIZE + PIVOT_Y - (BLOCK_SIZE * 2);
        g_stack[i]->setX(x);
        g_stack[i]->setY(y);
        g_stack[i]->setHovered(false); //~ 호버링 효과 제거 (이렇게 해줘야 크기 g_stack에서 이상하게 안변함)
    }
}

//! update 함수 :  g_stack의 객체들이 7개가 되면 g_vector의 객체들을 비활성화
void UpdateVectorBlocks() {
    for (auto& block : g_vector) {
        block->handleClick();
        block->update();

        if (g_stack.size() == 7) {
            block->setClickable(false);
        }
    }
}

//! update 함수 :  g_stack의 객체들이 7개가 되면 g_vector의 객체들을 비활성화
void UpdateStackBlocks() {
    for (auto& block : g_stack) {
        block->handleClick();
        block->update();
    }
}

//! update 함수 :  bonk 객체 업데이트
void UpdateBonks() {
    for (auto it = g_bonks.begin(); it != g_bonks.end();) {
        it->update();
        if (it->isTimeToDestroy()) {
            it = g_bonks.erase(it);
        }
        else {
            ++it;
        }
    }
}

//! update 함수 :  점수 업데이트
//~ 미구현
void UpdateScore(int score) {
}

//! Sort 함수 : 퀵 정렬을 위한 비교 함수
bool compareBlocks(const std::unique_ptr<Mahjong>& a, const std::unique_ptr<Mahjong>& b) {
    if (a->getType() == b->getType()) {
        return a->getX() < b->getX();
    }
    return a->getType() < b->getType();
}

//! Sort 함수 : 퀵 정렬 함수
void quickSort(std::vector<std::unique_ptr<Mahjong>>& blocks, int left, int right) {
    if (left >= right) {
        return;
    }

    int pivot = left;
    int i = left + 1;
    int j = right;

    while (i <= j) {
        while (i <= right && compareBlocks(blocks[i], blocks[pivot])) {
            i++;
        }
        while (j > left && !compareBlocks(blocks[j], blocks[pivot])) {
            j--;
        }

        if (i > j) {
            swap(blocks[pivot], blocks[j]);
        }
        else {
            swap(blocks[i], blocks[j]);
        }
    }

    quickSort(blocks, left, j - 1);
    quickSort(blocks, j + 1, right);
}

//! Sort 함수 : 쌍이 2개 이상인 객체들만 정렬하는 함수
void sortPairedBlocks() {
    std::map<std::string, int> typeCount;
    for (const auto& block : g_stack) {
        typeCount[block->getType()]++;
    }

    std::vector<std::unique_ptr<Mahjong>> pairedBlocks;
    std::vector<std::unique_ptr<Mahjong>> singleBlocks;

    for (auto& block : g_stack) {
        if (typeCount[block->getType()] >= 2) {
            pairedBlocks.push_back(std::move(block));
        }
        else {
            singleBlocks.push_back(std::move(block));
        }
    }

    quickSort(pairedBlocks, 0, pairedBlocks.size() - 1);

    g_stack.clear();
    for (auto& block : pairedBlocks) {
        g_stack.push_back(std::move(block));
    }
    for (auto& block : singleBlocks) {
        g_stack.push_back(std::move(block));
    }
}

//! 게임 종료 시 메모리 꼭! 해제
void ClearGame() {
    for (auto& block : g_vector) {
        block->destroyTexture();
        block->Clear2Sound();
    }
    g_vector.clear();
    for (auto& block : g_stack) {
        block->destroyTexture();
        block->Clear2Sound();
    }
    g_vector.clear();
    g_stack.clear();
    g_bonks.clear();

    Mix_FreeMusic(music1_);
    Mix_FreeChunk(wave1_);

    TTF_CloseFont(game_font_);
}