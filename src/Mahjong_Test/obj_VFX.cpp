// obj_VFX.cpp
#include "obj_VFX.h"

SDL_Texture* bonk::m_texture = nullptr;

bonk::bonk(int x, int y, SDL_Renderer* renderer)
    : m_x(x), m_y(y), m_frame(0), m_frameCount(VFX_FRAME), m_frameDelay(VFX_FRAME_DELAY), m_frameTimer(0) {
    if (!m_texture) {
        SDL_Surface* surface = IMG_Load("../Resources/bonkEffect.png");
        m_texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    for (int i = 0; i < m_frameCount; ++i) {
        m_sourceRects[i] = { i * 100, 0, 100, 100 };
    }

    m_createTime = SDL_GetTicks();
    cout << "bonk! ON" << endl;
}

bonk::~bonk() {
    cout << "bonk! OFF" << endl;
}

void bonk::update() {
    ++m_frameTimer;
    if (m_frameTimer >= m_frameDelay) {
        m_frameTimer = 0;
        ++m_frame;
    }
}

void bonk::render(SDL_Renderer* renderer) const {
    SDL_Rect dstRect = { m_x, m_y, 100, 100 };
    SDL_RenderCopy(renderer, m_texture, &m_sourceRects[m_frame], &dstRect);
}

bool bonk::isTimeToDestroy() const {
    Uint32 currentTime = SDL_GetTicks();
    return (currentTime - m_createTime) >= VFX_LIFETIME;  // 300ms ÈÄ¿¡ ¼Ò¸ê
}