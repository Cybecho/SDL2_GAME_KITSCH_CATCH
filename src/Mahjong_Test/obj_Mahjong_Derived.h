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

//! Mahjong_C
class Mahjong_C : public Mahjong {
public:
    Mahjong_C(int x, int y, SDL_Renderer* renderer);
    ~Mahjong_C();
};

//! Mahjong_D
class Mahjong_D : public Mahjong {
public:
    Mahjong_D(int x, int y, SDL_Renderer* renderer);
    ~Mahjong_D();
};