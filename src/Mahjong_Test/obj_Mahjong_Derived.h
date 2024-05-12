#pragma once

#include "obj_Mahjong_Base.h"

class Mahjong_A : public Mahjong {
public:
    Mahjong_A(int x, int y, SDL_Renderer* renderer);
    ~Mahjong_A();
    void update() override;
    void render(SDL_Renderer* renderer) const override;
    static void destroyTexture();
    bool isOutOfScreen() override;
    static void loadTexture(SDL_Renderer* renderer);
    static void Set2Sound();
    static void Clear2Sound();

private:
    int m_speed;
    int m_frame;
    int m_frameCount;
    int m_frameDelay;
    int m_frameTimer;

    static SDL_Texture* m_texture;
    SDL_Rect m_sourceRects[3];

    static Mix_Chunk* m_sound;
};