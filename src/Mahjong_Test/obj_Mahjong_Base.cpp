#include "obj_Mahjong_Base.h"

Mix_Chunk* Mahjong::m_sound = nullptr;
SDL_Texture* Mahjong::m_texture = nullptr;

Mahjong::Mahjong(int x, int y, SDL_Renderer* renderer, const SDL_Rect& sourceRect)
    : m_x(x), m_y(y), m_speed(FIRE_SPEED), clicked(false), m_sourceRect(sourceRect) {
    if (!m_texture) {
        loadTexture(renderer);
    }

    if (!m_sound) {
        Set2Sound();
    }

    Mix_PlayChannel(-1, m_sound, 0);

    cout << "Mahjong Create" << " x : " << this->m_x << " y : " << this->m_y << endl;
}

Mahjong::~Mahjong() {
    cout << "Mahjong Delete" << endl;
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
    return x >= m_x && x < m_x + BLOCK_SIZE && y >= m_y && y < m_y + BLOCK_SIZE;
}

void Mahjong::Set2Sound() {
    if (!m_sound) {
        m_sound = Mix_LoadWAV("../Resources/Bullett_Classic.mp3");
        if (!m_sound) {
            printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        }
        else {
            Mix_VolumeChunk(m_sound, 32);
        }
    }
}

void Mahjong::Clear2Sound() {
    if (m_sound) {
        Mix_FreeChunk(m_sound);
        m_sound = nullptr;
    }
}

void Mahjong::render(SDL_Renderer* renderer) const {
    SDL_Rect dstRect = { m_x, m_y, BLOCK_SIZE, BLOCK_SIZE };
    SDL_RenderCopy(renderer, m_texture, &m_sourceRect, &dstRect);
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