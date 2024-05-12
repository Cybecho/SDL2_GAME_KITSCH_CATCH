#pragma once

#include "obj_Mahjong_Base.h"

//! Mahjong_A
class Mahjong_A : public Mahjong {
public:
    Mahjong_A(int x, int y, SDL_Renderer* renderer);
    ~Mahjong_A();
};

//! Mahjong_B
class Mahjong_B : public Mahjong {
public:
    Mahjong_B(int x, int y, SDL_Renderer* renderer);
    ~Mahjong_B();
};