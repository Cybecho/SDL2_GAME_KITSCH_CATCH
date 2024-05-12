#include "obj_Mahjong_Base.h"

Mix_Chunk* Mahjong::m_sound = nullptr;
SDL_Texture* Mahjong::m_texture = nullptr;

Mahjong::Mahjong(int x, int y, SDL_Renderer* renderer, const SDL_Rect& sourceRect)
    : m_x(x), m_y(y), clicked(false), m_sourceRect(sourceRect),m_blockSize(BLOCK_SIZE), m_blockScale(BLOCK_SCALE) {
    if (!m_texture) {
        loadTexture(renderer);
    }

    if (!m_sound) {
        Set2Sound();
    }

    cout << "Mahjong Create" << " x : " << this->m_x << " y : " << this->m_y << endl;
}

void Mahjong::update() {
}

void Mahjong::handleClick() {
    if (clicked) {
        m_x = 0;
        m_y = 0;
    }
}

bool Mahjong::isClicked(int x, int y) const {
    return x >= m_x && x < m_x + (m_blockSize / m_blockScale) && y >= m_y && y < m_y + (m_blockSize / m_blockScale);
}

void Mahjong::Set2Sound() {
    if (!m_sound) {
        m_sound = Mix_LoadWAV("../Resources/ClickBlock.mp3");
        if (!m_sound) {
            printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        }
        else {
            Mix_VolumeChunk(m_sound, 32);
        }
    }
}

void Mahjong::Play2Sound() {
    if (m_sound) {
        Mix_PlayChannel(-1, m_sound, 0);
    }
}

void Mahjong::Clear2Sound() {
    if (m_sound) {
        Mix_FreeChunk(m_sound);
        m_sound = nullptr;
    }
}

void Mahjong::render(SDL_Renderer* renderer) const {
    SDL_Rect dstRect = { m_x, m_y, static_cast<int>(m_blockSize / m_blockScale), static_cast<int>(m_blockSize / m_blockScale) };

    if (clickEnable) { //! 클릭 가능한 경우
        SDL_RenderCopy(renderer, m_texture, &m_sourceRect, &dstRect);
    }
    else { //! 클릭 불가능한 경우
        Uint8 alpha = 128; // 반투명도 설정 (0-255)
        SDL_SetTextureAlphaMod(m_texture, alpha); // 투명도 설정
        SDL_RenderCopy(renderer, m_texture, &m_sourceRect, &dstRect);
        SDL_SetTextureAlphaMod(m_texture, 255); // 원래 투명도로 복구
    }
}

void Mahjong::destroyTexture() {
    SDL_DestroyTexture(m_texture);
    m_texture = nullptr;
}

void Mahjong::loadTexture(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("../Resources/Mahjong.png");
    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}