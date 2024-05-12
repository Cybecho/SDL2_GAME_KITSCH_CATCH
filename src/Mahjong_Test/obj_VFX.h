// obj_VFX.h
#pragma once
#define VFX_FRAME 9
#define VFX_FRAME_DELAY 1
#define VFX_LIFETIME 300
#include "Include.h"

class bonk {
public:
    bonk(int x, int y, SDL_Renderer* renderer);
    ~bonk();
    void update();
    void render(SDL_Renderer* renderer) const;
    bool isTimeToDestroy() const;

private:
    int m_x;
    int m_y;
    int m_frame;
    int m_frameCount;
    int m_frameDelay;
    int m_frameTimer;
    Uint32 m_createTime;
    SDL_Rect m_sourceRects[VFX_FRAME];
    static SDL_Texture* m_texture;
};