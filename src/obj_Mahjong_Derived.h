#pragma once

#include "obj_Mahjong_Base.h"

//! Mahjong_A
class Mahjong_A : public Mahjong {
public:
    Mahjong_A(int x, int y, SDL_Renderer* renderer);
    ~Mahjong_A();
    string getType() const override { return "A"; }
};

//! Mahjong_B
class Mahjong_B : public Mahjong {
public:
    Mahjong_B(int x, int y, SDL_Renderer* renderer);
    ~Mahjong_B();
    string getType() const override { return "B"; }
};

//! Mahjong_C
class Mahjong_C : public Mahjong {
public:
    Mahjong_C(int x, int y, SDL_Renderer* renderer);
    ~Mahjong_C();
    string getType() const override { return "C"; }
};

//! Mahjong_D
class Mahjong_D : public Mahjong {
public:
    Mahjong_D(int x, int y, SDL_Renderer* renderer);
    ~Mahjong_D();
    string getType() const override { return "D"; }
};

//! Mahjong_Empty
class Mahjong_Empty : public Mahjong {
public:
    Mahjong_Empty(int x, int y, SDL_Renderer* renderer);
    ~Mahjong_Empty();
    string getType() const override { return "E"; }
};