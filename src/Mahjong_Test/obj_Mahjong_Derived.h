#pragma once

#include "obj_Mahjong_Base.h"

//! Mahjong_A
class Mahjong_A : public Mahjong {
public:
    Mahjong_A(int x, int y, SDL_Renderer* renderer);
    ~Mahjong_A();

private:
    void loadTexture(SDL_Renderer* renderer) override;
};


//! Mahjong_B
class Mahjong_B : public Mahjong {
public:
    Mahjong_B(int x, int y, SDL_Renderer* renderer);
    ~Mahjong_B();

private:
    void loadTexture(SDL_Renderer* renderer) override;
};