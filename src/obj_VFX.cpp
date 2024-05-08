#include "obj_VFX.h"

SDL_Texture* Fire::m_texture = nullptr;
Mix_Chunk* Fire::m_sound = nullptr;

bonk::bonk() {
    mX = 0;
    mY = 0;
}

bonk::bonk(int x, int y) {
    mX = x;
    mY = y;
}

~bonk::bonk() {
    
}