// DrawingTechniques_GameFunc.cpp
#include "DrawingTechniques_GameFunc.h"
#include "obj_Mahjong_Derived.h"
#include "obj_VFX.h"

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

int g_scale;

void InitGame() {
    g_flag_running = true;

    g_scale = BLOCK_SCALE;

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
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            
            g_vector.emplace_back(std::make_unique<Mahjong_A>(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, g_renderer));
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouse_x = event.button.x;
            int mouse_y = event.button.y;

            for (auto it = g_vector.begin(); it != g_vector.end(); ++it) {
                if ((*it)->isClicked(mouse_x, mouse_y)) {
                    (*it)->setClicked(true);
                    (*it)->Play2Sound();
                    cout << "Clicked! : " << mouse_x << " , " << mouse_y << endl;

                    //! g_blocks와 g_stack 간 swap
                    auto block = std::move(*it);
                    g_vector.erase(it);
                    g_stack.emplace_back(std::move(block));

                    //! g_blocks와 g_stack의 사이즈 출력
                    std::cout << "g_blocks size: " << g_vector.size() << "| g_stack size: " << g_stack.size() << std::endl;

                    // bonk 객체 생성
                    if (BLOCK_SCALE == 1) {
                        g_bonks.emplace_back(mouse_x - (BLOCK_SIZE / 2), mouse_y - (BLOCK_SIZE / 2), g_renderer);
                    }
                    else {
                        g_bonks.emplace_back(mouse_x - (BLOCK_SIZE / (BLOCK_SCALE * BLOCK_SCALE)), mouse_y - (BLOCK_SIZE / (BLOCK_SCALE * BLOCK_SCALE)), g_renderer);
                    }
                    break;
                }
            }
        }
    }
}

void Update() {
    // 마작 객체 업데이트
    for (auto& block : g_vector) {
        block->handleClick();
        block->update();
    }
    for (auto& block : g_stack) {
        block->handleClick();
        block->update();
    }

    // bonk 객체 업데이트
    for (auto it = g_bonks.begin(); it != g_bonks.end();) {
        it->update();
        if (it->isTimeToDestroy()) {
            it = g_bonks.erase(it);
        }
        else {
            ++it;
        }
    }

    //! 벡터 영역이 비었을 때 csv 파일을 읽어와서 다시 로드
    if (g_vector.empty()) {
        if (g_vector.empty()) {
            int level = 0;
            int seed = 0;
            int numDims = 3;
            LoadMahjongBlocksFromCSV(level, seed, numDims);
        }
    }
}

void Render() {
    SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
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

void LoadMahjongBlocksFromCSV(int level, int seed, int numDims) {
    g_vector.clear();

    for (int dim = 0; dim < numDims; ++dim) {
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