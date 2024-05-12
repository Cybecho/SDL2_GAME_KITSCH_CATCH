#pragma once
#include "Include.h"
#include "obj_VFX.h"

#define BLOCK_SIZE 100

class Mahjong {
public:
    Mahjong(int x, int y, SDL_Renderer* renderer, const SDL_Rect& sourceRect);
    virtual ~Mahjong();
    void update();
    void render(SDL_Renderer* renderer) const;
    static void destroyTexture();
    static void Set2Sound();
    static void Play2Sound();
    static void Clear2Sound();

    bool isClicked(int x, int y) const;
    void setClicked(bool value) { clicked = value; }
    void handleClick();

protected:
    static void loadTexture(SDL_Renderer* renderer);
    int m_x;
    int m_y;
    bool clicked;
    SDL_Rect m_sourceRect;
    static Mix_Chunk* m_sound;
    static SDL_Texture* m_texture;
};