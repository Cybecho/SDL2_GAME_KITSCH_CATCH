// DrawingTechniques_GameFunc.cpp
#include "DrawingTechniques_GameFunc.h"
#include "obj_Mahjong_Derived.h"
#include "obj_VFX.h"

std::vector<Mahjong_A> g_blocks; //! 마작 블록 생성 벡터
std::vector<bonk> g_bonks; //! 효과 블록 생성 벡터

Mix_Chunk* wave1_;
Mix_Music* music1_;

SDL_Texture* text_score_;
SDL_Rect text_score_rect_;
TTF_Font* game_font_;

SDL_Rect g_bg_source_rect;
SDL_Rect g_bg_destination_rect;

int g_score;
int g_input;
int g_blockCount = BLOCK_SIZE;

void InitGame() {
    g_flag_running = true;

    g_input = 0;
    g_score = 0;

    g_bg_source_rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    g_bg_destination_rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
}

void HandleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            if (g_blocks.size() < g_blockCount) {
                g_blocks.emplace_back(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2, g_renderer);
                cout << "MAGAZINE : " << g_blocks.size() << "/" << BLOCK_SIZE << endl;
                g_score += 10;
            }
            else {
                cout << "Maximum fire count reached!" << endl;
            }
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x = event.button.x;
            int y = event.button.y;

            for (auto& block : g_blocks) {
                if (block.isClicked(x, y)) {
                    block.setClicked(true);
                    block.Play2Sound();
                    cout << "Clicked!" << endl;

                    // 클릭된 위치에 bonk 객체 생성
                    g_bonks.emplace_back(x - (BLOCK_SIZE/2), y - (BLOCK_SIZE / 2), g_renderer);
                    break;
                }
            }
        }
    }
}

void Update() {
    //! 마작 객체 업데이트
    for (auto& block : g_blocks) {
        block.handleClick();
        block.update();
    }

    //! bonk 객체 업데이트
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

void Render() {
    SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
    SDL_RenderClear(g_renderer);

    //! 블록 객체 렌더링
    for (const auto& block : g_blocks) {
        block.render(g_renderer);
    }

    //! bonk 객체 렌더링
    for (const auto& bonk : g_bonks) {
        bonk.render(g_renderer);
    }

    SDL_RenderPresent(g_renderer);
}

void ClearGame() {
    for (auto& block : g_blocks) {
        block.destroyTexture();
        block.Clear2Sound();
    }
    g_blocks.clear();
    g_bonks.clear();

    Mix_FreeMusic(music1_);
    Mix_FreeChunk(wave1_);

    TTF_CloseFont(game_font_);
}