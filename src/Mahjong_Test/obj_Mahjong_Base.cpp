#include "obj_Mahjong_Base.h"

Mix_Chunk* Mahjong::m_sound = nullptr;
SDL_Texture* Mahjong::m_texture = nullptr;

Mahjong::Mahjong(int x, int y, SDL_Renderer* renderer) : m_x(x), m_y(y), m_speed(FIRE_SPEED), m_frame(0), m_frameCount(3), m_frameDelay(5), m_frameTimer(0) {
    if (!m_texture) {
        loadTexture(renderer);
    }

    if (!m_sound) {
        Set2Sound();
    }

    Mix_PlayChannel(-1, m_sound, 0);

    for (int i = 0; i < m_frameCount; ++i) {
        m_sourceRects[i] = { i * 200, 0, 200, 200 };
    }

    cout << "Mahjong Create" << " x : " << this->m_x << " y : " << this->m_y << endl;
}

Mahjong::~Mahjong() {
    cout << "Mahjong Delete" << endl;
}

void Mahjong::update() {
    m_y -= m_speed;

    ++m_frameTimer;
    if (m_frameTimer >= m_frameDelay) {
        m_frameTimer = 0;
        ++m_frame;
        if (m_frame >= m_frameCount) {
            m_frame = 0;
        }
    }
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
    SDL_Rect dstRect = { m_x - 100, m_y, 200, 200 };
    SDL_RenderCopy(renderer, m_texture, &m_sourceRects[m_frame], &dstRect);
}

bool Mahjong::isOutOfScreen() {
    return m_y < -200;
}

void Mahjong::destroyTexture() {
    SDL_DestroyTexture(m_texture);
    m_texture = nullptr;
}