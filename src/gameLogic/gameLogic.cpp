#include "gameLogic.h"

vector<unique_ptr<Mahjong>> g_vector; // 마작 블록 생성 벡터
vector<unique_ptr<Mahjong>> g_stack; // 마작 블록 스택 벡터
vector<bonk> g_bonks; // 효과 블록 생성 벡터

Mix_Chunk* wave1_;
Mix_Music* music1_;

SDL_Texture* text_score_;
SDL_Rect text_score_rect_;
TTF_Font* game_font_;

SDL_Rect g_bg_source_rect;
SDL_Rect g_bg_destination_rect;

int g_level;
int MAX_LEVEL = countDir("../../res/level") - 1; // 최대 레벨 수는 디렉토리 개수 - 1 (for문 때문에 -1 처리를 해줌)
int g_status = STATUS_GAMEPLAYING;

void InitGame() {
    g_flag_running = true;
    g_status = STATUS_GAMEPLAYING;
    g_level = 0;

    g_bg_source_rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    g_bg_destination_rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
}

void HandleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        //! 스페이스바 누르면 벡터&스택 초기화
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == 'r') {
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

            // g_vector를 역순으로 순회하여 상위 레이어부터 클릭 이벤트 처리
            for (auto it = g_vector.rbegin(); it != g_vector.rend(); ++it) {
                //! clickEnable이 true이고 Mahjong_Empty 블록이 아닌 경우에만 클릭 이벤트 처리
                if ((*it)->isClickable() && !dynamic_cast<Mahjong_Empty*>(it->get()) && (*it)->isClicked(mouse_x, mouse_y)) {
                    (*it)->Play2Sound();

                    vector2stack(it.base() - 1); //! 클릭된 블록을 g_stack으로 이동
                    createBonk(mouse_x, mouse_y); //! 클릭 위치에 bonk 객체 생성
                    break;
                }
            }
        }
    }
}

void Update() {
    LoadMahjongBlocksIfEmpty(g_level);
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
        string filename = "../../res/level/" + to_string(level) + "/" + to_string(seed) + "-" + to_string(dim) + ".csv";
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Failed to open file: " << filename << endl;
            continue;
        }
        string line;
        int row = 0;

        while (getline(file, line)) {
            istringstream iss(line);
            string cell;
            int col = 0;

            while (getline(iss, cell, ',')) {
                int data = stoi(cell);

                int x = col * BLOCK_SIZE + PIVOT_X + (dim * BLOCK_SIZE / 2);
                int y = row * BLOCK_SIZE + PIVOT_Y + (dim * BLOCK_SIZE / 2);

                if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) {
                    switch (data) {
                    case 1:
                        g_vector.emplace_back(make_unique<Mahjong_A>(x, y, g_renderer));
                        g_vector.back()->setPosition(dim, row, col);
                        break;
                    case 2:
                        g_vector.emplace_back(make_unique<Mahjong_B>(x, y, g_renderer));
                        g_vector.back()->setPosition(dim, row, col);
                        break;
                    case 3:
                        g_vector.emplace_back(make_unique<Mahjong_C>(x, y, g_renderer));
                        g_vector.back()->setPosition(dim, row, col);
                        break;
                    case 4:
                        g_vector.emplace_back(make_unique<Mahjong_D>(x, y, g_renderer));
                        g_vector.back()->setPosition(dim, row, col);
                        break;
                    default:
                        g_vector.emplace_back(make_unique<Mahjong_Empty>(x, y, g_renderer));
                        g_vector.back()->setPosition(dim, row, col);
                        break;
                    }
                }

                ++col;
            }

            ++row;
            cout << endl; // 줄바꿈
        }
    }

    //! 대각선 방향으로 블록 흔들림 효과 적용
    int totalBlocks = g_vector.size();
    for (int i = 0; i < totalBlocks; ++i) {
        int delay = i * 3; // 0.03초(3ms) 지연 시간
        SDL_AddTimer(delay, [](Uint32 interval, void* param) -> Uint32 {
            int index = *(static_cast<int*>(param));
            g_vector[index]->shakeBlocks(5); // 애니메이션 지속시간
            delete static_cast<int*>(param);
            return 0;
            }, new int(i));
    }
}

//! 클릭된 블록을 g_vector에서 g_stack으로 이동
void vector2stack(vector<unique_ptr<Mahjong>>::iterator it) {
    auto block = move(*it);
    g_vector.erase(it);

    // 객체의 위치를 g_stack의 크기에 따라 동적으로 계산합니다.
    int x = g_stack.size() * (BLOCK_SIZE / 2) + PIVOT_X;
    int y = BLOCK_SIZE + PIVOT_Y - (BLOCK_SIZE * 2);

    block->setX(x);
    block->setY(y);

    g_stack.emplace_back(move(block));

    // g_blocks와 g_stack의 사이즈 출력
    cout << "g_blocks size: " << g_vector.size() - countEmptyBlocks() << "| g_stack size: " << g_stack.size() << endl;
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
void LoadMahjongBlocksIfEmpty(int level) {
    if (checkEmptyBlocks()) {
        for (auto it = g_vector.begin(); it != g_vector.end();) {
            if (dynamic_cast<Mahjong_Empty*>(it->get())) {
                it = g_vector.erase(it);
            }
            else {
                ++it;
            }
        }

        //~ 레벨 증가 로직
        if (level < MAX_LEVEL) { g_level++; }   //~ g_level이 최대 레벨이 아닐 경우 레벨 증가
        else { g_status = STATUS_GAMECLEAR; }       //~ g_level이 최대 레벨일 경우 게임오버
        srand(time(NULL));
        string dir_path = "../../res/level/" + to_string(level);
        int seed = rand() % (countFiles(dir_path) / 3);  //~ 현재 레벨에서 발생 가능한 시드 (전체파일수 / 3)
        int numDims = 2;
        LoadMahjongBlocksFromCSV(level, seed, numDims); 
    }
}

//! update 함수 :  g_stack에서 같은 객체가 3개 이상 존재할 경우 pop 처리
void RemoveSameTypeBlocks() {
    map<string, vector<int>> typeIndices;
    for (int i = 0; i < g_stack.size(); ++i) {
        if (dynamic_cast<Mahjong_Empty*>(g_stack[i].get()) == nullptr) {
            typeIndices[g_stack[i]->getType()].push_back(i);
        }
    }

    bool blocksRemoved = false;

    for (auto it = typeIndices.rbegin(); it != typeIndices.rend(); ++it) {
        if (it->second.size() >= 3) {
            int count = 0;
            for (int i = it->second.size() - 1; i >= 0; --i) {
                if (count < 3) {
                    int index = it->second[i];
                    // 제거되는 블록의 위치에서 createBonk() 호출
                    int x = g_stack[index]->getX() + BLOCK_SIZE / 2;
                    int y = g_stack[index]->getY() + BLOCK_SIZE / 2;
                    createBonk(x, y);
                    g_stack.erase(g_stack.begin() + index);
                    ++count;
                    blocksRemoved = true;
                }
                else {
                    break;
                }
            }
        }
    }

    // 블록이 제거된 경우에만 shakeBlocks() 호출
    if (blocksRemoved) {
        for (auto& block : g_stack) {
            block->shakeBlocks(10);
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
void UpdateVectorBlocks()
{
    for (auto& block : g_vector)
    {
        block->handleClick();
        block->update();
        block->checkClickEnable(g_vector); // g_vector를 매개변수로 전달합니다.

        if (g_stack.size() == 7)
        {
            block->setClickable(false);
            g_status = STATUS_GAMEOVER;
        }
    }
}

//! update 함수 :  g_stack의 객체들이 7개가 되면 g_stack의 객체들을 비활성화
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

//! Empty 마작 블록이 비었는지 체크
bool checkEmptyBlocks() {
    //~ g_vector가 비어있거나 모든 블록이 Empty 블록인 경우 true 반환
    return g_vector.empty() || all_of(g_vector.begin(), g_vector.end(), [](const auto& block) {
        return dynamic_cast<Mahjong_Empty*>(block.get()) != nullptr; //~ Empty 블록인지 체크
        });
}

//! Empty 마작 블록 개수를 세는 함수
int countEmptyBlocks() {
    return count_if(g_vector.begin(), g_vector.end(), [](const auto& block) {
        return dynamic_cast<Mahjong_Empty*>(block.get()) != nullptr;
        });
}

//! Sort 함수 : 퀵 정렬을 위한 비교 함수
bool compareBlocks(const unique_ptr<Mahjong>& a, const unique_ptr<Mahjong>& b) {
    if (a->getType() == b->getType()) {
        return a->getX() < b->getX();
    }
    return a->getType() < b->getType();
}

//! Sort 함수 : 퀵 정렬 함수
void quickSort(vector<unique_ptr<Mahjong>>& blocks, int left, int right) {
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
    map<string, int> typeCount;
    for (const auto& block : g_stack) {
        typeCount[block->getType()]++;
    }

    vector<unique_ptr<Mahjong>> pairedBlocks;
    vector<unique_ptr<Mahjong>> singleBlocks;

    for (auto& block : g_stack) {
        if (typeCount[block->getType()] >= 2) {
            pairedBlocks.push_back(move(block));
        }
        else {
            singleBlocks.push_back(move(block));
        }
    }

    quickSort(pairedBlocks, 0, pairedBlocks.size() - 1);

    g_stack.clear();
    for (auto& block : pairedBlocks) {
        g_stack.push_back(move(block));
    }
    for (auto& block : singleBlocks) {
        g_stack.push_back(move(block));
    }
}

//! 디렉토리 내 폴더 개수 세기
int countDir(const string& path) {
    int count = 0;
    wstring search_path = wstring(path.begin(), path.end()) + L"\\*";
    WIN32_FIND_DATAW fd;
    HANDLE hFind = FindFirstFileW(search_path.c_str(), &fd);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                (wcscmp(fd.cFileName, L".") != 0) &&
                (wcscmp(fd.cFileName, L"..") != 0)) {
                ++count;
            }
        } while (FindNextFileW(hFind, &fd));
        FindClose(hFind);
    }

    cout << path << " dir count: " << count << endl;
    return count;
}

//! 디렉토리 내 파일 개수 세기
int countFiles(const string& path) {
    int count = 0;
    wstring search_path = wstring(path.begin(), path.end()) + L"\\*";
    WIN32_FIND_DATAW fd;
    HANDLE hFind = FindFirstFileW(search_path.c_str(), &fd);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                ++count;
            }
        } while (FindNextFileW(hFind, &fd));
        FindClose(hFind);
    }
    cout << path << " csv count: " << count << endl;
    return count;
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