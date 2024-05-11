#include "obj_VFX.h"

SDL_Texture* bonk::m_texture = nullptr;
Mix_Chunk* bonk::m_sound = nullptr;

bonk::bonk() {
    mX = 0;
    mY = 0;
}

bonk::bonk(int x, int y, SDL_Renderer* renderer)
    {
    mX = x;
    mY = y;

    if (!m_texture) {loadTexture(renderer);}
    if (!m_sound) {Set2Sound();}
    Mix_PlayChannel(-1, m_sound, 0);

    for (int i = 0; i < m_frameCount; ++i) {
        m_sourceRects[i] = { i * 200, 0, 200, 200 };
    }
    cout << "bonk Create" << " x : " << mX << " y : " << mY << endl;
}

bonk::~bonk() {
    cout << "bonk Destroy" << " x : " << mX << " y : " << mY << endl;
}