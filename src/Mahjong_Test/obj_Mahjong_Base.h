#pragma once

#include "Include.h"

class Mahjong {
public:
    Mahjong(int x, int y, SDL_Renderer* renderer, const SDL_Rect& sourceRect);
    virtual ~Mahjong();
    void update();
    void render(SDL_Renderer* renderer) const;
    bool isOutOfScreen();
    static void destroyTexture();
    static void Set2Sound();
    static void Clear2Sound();

protected:
    static void loadTexture(SDL_Renderer* renderer);
    int m_x;
    int m_y;
    int m_speed;
    SDL_Rect m_sourceRect;
    static Mix_Chunk* m_sound;
    static SDL_Texture* m_texture;
};