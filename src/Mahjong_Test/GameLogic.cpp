#include "GameLogic.h"

vector<vector<vector<unique_ptr<Mahjong>>>> g_vector; // 마작 블록 생성 벡터
vector<unique_ptr<Mahjong>> g_stack; // 마작 블록 스택 벡터
vector<bonk> g_bonks; // 효과 블록 생성 벡터

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
    int max_level = countDir("../../res/level");    //~ 최대 레벨
    int cur_level = 0;                              //~ 시작 레벨
    int seed = 0;								    //~ 현재 레벨에서 발생 가능한 시드
    int numDims = 3;
    for (int i = 0; i < max_level; i++)
    {
        std::string dir_path = "../../res/level/" + std::to_string(i);
        countFiles(dir_path);
    }
    LoadMahjongBlocksFromCSV(cur_level, seed, numDims);
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

            for (int dim = 0; dim < g_vector.size(); ++dim) {
                for (int row = 0; row < g_vector[dim].size(); ++row) {
                    for (int col = 0; col < g_vector[dim][row].size(); ++col) {
                        Mahjong* block = g_vector[dim][row][col].get();
                        if (block && block->isClickable()) {
                            block->setHovered(block->isHovered(mouse_x, mouse_y));
                        }
                    }
                }
            }
        }

        //! 마우스 클릭 이벤트 처리
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouse_x = event.button.x;
            int mouse_y = event.button.y;

            for (int dim = g_vector.size() - 1; dim >= 0; --dim) {
                for (int row = g_vector[dim].size() - 1; row >= 0; --row) {
                    for (int col = g_vector[dim][row].size() - 1; col >= 0; --col) {
                        Mahjong* block = g_vector[dim][row][col].get();
                        if (block && block->isClickable() && block->isClicked(mouse_x, mouse_y)) {
                            block->setClicked(true);
                            block->handleClick();
                            vector2stack(g_vector[dim][row][col].get()); // 반복자 대신 get()으로 포인터를 전달
                            return;
                        }
                    }
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
    for (int dim = 0; dim < g_vector.size(); ++dim) {
        for (int row = 0; row < g_vector[dim].size(); ++row) {
            for (int col = 0; col < g_vector[dim][row].size(); ++col) {
                Mahjong* block = g_vector[dim][row][col].get();
                if (block) {
                    block->render(g_renderer);
                }
            }
        }
    }

    // 스택 객체 렌더링
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
    g_vector.resize(numDims);

    for (int dim = 0; dim < numDims; ++dim) {
        string filename = "../../res/level/" + to_string(level) + "/" + to_string(seed) + "-" + to_string(dim) + ".csv";
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            continue;
        }
        string line;
        int row = 0;

        while (getline(file, line)) {
            g_vector[dim].resize(row + 1); // 행의 크기 지정
            istringstream iss(line);
            string cell;
            int col = 0;

            while (std::getline(iss, cell, ',')) {
                int data = std::stoi(cell);

                int x = col * BLOCK_SIZE + PIVOT_X + (dim * BLOCK_SIZE / 2);
                int y = row * BLOCK_SIZE + PIVOT_Y + (dim * BLOCK_SIZE / 2);

                if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) {
                    switch (data) {
<<<<<<< HEAD
                    case 1: {
                        auto block = std::make_unique<Mahjong_A>(x, y, g_renderer);
                        block->setN(dim);
                        block->setM(row);
                        block->setR(col);
                        g_vector[dim][row].emplace_back(std::move(block));
                        break;
                    }
                    case 2: {
                        auto block = std::make_unique<Mahjong_B>(x, y, g_renderer);
                        block->setN(dim);
                        block->setM(row);
                        block->setR(col);
                        g_vector[dim][row].emplace_back(std::move(block));
                        break;
                    }
                    case 3: {
                        auto block = std::make_unique<Mahjong_C>(x, y, g_renderer);
                        block->setN(dim);
                        block->setM(row);
                        block->setR(col);
                        g_vector[dim][row].emplace_back(std::move(block));
                        break;
                    }
                    case 4: {
                        auto block = std::make_unique<Mahjong_D>(x, y, g_renderer);
                        block->setN(dim);
                        block->setM(row);
                        block->setR(col);
                        g_vector[dim][row].emplace_back(std::move(block));
=======
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
>>>>>>> parent of 7387946 (선택 제한 로직 구현 2 (미완))
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

    //! 대각선 방향으로 블록 흔들림 효과 적용
    int totalBlocks = 0;
    for (const auto& dim : g_vector) {
        for (const auto& row : dim) {
            totalBlocks += row.size();
        }
    }

    for (int i = 0; i < totalBlocks; ++i) {
        int delay = i * 3; // 0.03초(3ms) 지연 시간
        SDL_AddTimer(delay, [](Uint32 interval, void* param) -> Uint32 {
            int index = *(static_cast<int*>(param));
            int count = 0;
            for (const auto& dim : g_vector) {
                for (const auto& row : dim) {
                    for (const auto& block : row) {
                        if (count == index) {
                            block->shakeBlocks(5); // 애니메이션 지속시간
                            delete static_cast<int*>(param);
                            return 0;
                        }
                        ++count;
                    }
                }
            }
            delete static_cast<int*>(param);
            return 0;
            }, new int(i));
    }
}

//! 클릭된 블록을 g_vector에서 g_stack으로 이동
void vector2stack(Mahjong* block) {
    int dim, row, col;
    tie(dim, row, col) = block->getClickedBlockInfo();

    auto& blockPtr = g_vector[dim][row][col];
    g_stack.emplace_back(std::move(blockPtr));
    blockPtr.reset();

    // 객체의 위치를 g_stack의 크기에 따라 동적으로 계산합니다.
    int x = g_stack.size() * (BLOCK_SIZE / 2) + PIVOT_X;
    int y = BLOCK_SIZE + PIVOT_Y - (BLOCK_SIZE * 2);
    g_stack.back()->setX(x);
    g_stack.back()->setY(y);

    // g_vector의 총 블록 수와 g_stack의 사이즈 출력
    int totalBlocks = 0;
    for (const auto& dim : g_vector) {
        for (const auto& row : dim) {
            totalBlocks += row.size();
        }
    }
    std::cout << "g_vector size: " << totalBlocks << "| g_stack size: " << g_stack.size() << std::endl;
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
    int totalBlocks = 0;
    for (const auto& dim : g_vector) {
        for (const auto& row : dim) {
            totalBlocks += row.size();
        }
    }

    if (totalBlocks == 0) {
        srand(time(NULL));
        int level = rand() % 5;
        int seed = rand() % countFiles("../../res/level/" + to_string(level));
        int numDims = 3;
        LoadMahjongBlocksFromCSV(level, seed, numDims);
    }
}

//! update 함수 :  g_stack에서 같은 객체가 3개 이상 존재할 경우 pop 처리
void RemoveSameTypeBlocks() {
    map<string, vector<int>> typeIndices;
    for (int i = 0; i < g_stack.size(); ++i) {
        typeIndices[g_stack[i]->getType()].push_back(i);
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
void UpdateVectorBlocks() {
    for (int dim = 0; dim < g_vector.size(); ++dim) {
        for (int row = 0; row < g_vector[dim].size(); ++row) {
            for (int col = 0; col < g_vector[dim][row].size(); ++col) {
                Mahjong* block = g_vector[dim][row][col].get();
                if (block) {
                    block->handleClick();
                    block->update();

                    if (g_stack.size() == 7) {
                        block->setClickable(false);
                    }
                }
            }
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

//! 디렉토리 내 폴더 개수 세기
int countDir(const std::string& path) {
    int count = 0;
    std::wstring search_path = std::wstring(path.begin(), path.end()) + L"\\*";
    WIN32_FIND_DATAW fd;
    HANDLE hFind = FindFirstFileW(search_path.c_str(), &fd);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                (std::wcscmp(fd.cFileName, L".") != 0) &&
                (std::wcscmp(fd.cFileName, L"..") != 0)) {
                ++count;
            }
        } while (FindNextFileW(hFind, &fd));
        FindClose(hFind);
    }

    std::cout << path << " dir count: " << count << std::endl;
    return count;
}

//! 디렉토리 내 파일 개수 세기
int countFiles(const std::string& path) {
    int count = 0;
    std::wstring search_path = std::wstring(path.begin(), path.end()) + L"\\*";
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
    for (int dim = 0; dim < g_vector.size(); ++dim) {
        for (int row = 0; row < g_vector[dim].size(); ++row) {
            for (int col = 0; col < g_vector[dim][row].size(); ++col) {
                Mahjong* block = g_vector[dim][row][col].get();
                if (block) {
                    block->destroyTexture();
                    block->Clear2Sound();
                }
            }
        }
    }
    g_vector.clear();
    g_stack.clear();
    g_bonks.clear();

    Mix_FreeMusic(music1_);
    Mix_FreeChunk(wave1_);

    TTF_CloseFont(game_font_);
}