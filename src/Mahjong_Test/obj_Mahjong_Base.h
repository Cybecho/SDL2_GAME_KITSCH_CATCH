#pragma once

#include "Include.h"

class Mahjong {
public:
    Mahjong(int x, int y, SDL_Renderer* renderer);
    virtual ~Mahjong();
    void update();
    void render(SDL_Renderer* renderer) const;
    bool isOutOfScreen();
    static void destroyTexture();
    static void Set2Sound();
    static void Clear2Sound();

protected:
    virtual void loadTexture(SDL_Renderer* renderer) = 0;
    int m_x;
    int m_y;
    int m_speed;
    int m_frame;
    int m_frameCount;
    int m_frameDelay;
    int m_frameTimer;
    SDL_Rect m_sourceRects[3];
    static Mix_Chunk* m_sound;
    static SDL_Texture* m_texture;
};