#include "obj_Mahjong_Derived.h"

//! Mahjong_A
Mahjong_A::Mahjong_A(int x, int y, SDL_Renderer* renderer)
    : Mahjong(x, y, renderer, { 0, 0, 200, 200 }) { cout << "Mahjong_A" << endl; }
Mahjong_A::~Mahjong_A() {}

//! Mahjong_B
Mahjong_B::Mahjong_B(int x, int y, SDL_Renderer* renderer)
    : Mahjong(x, y, renderer, { 200, 0, 200, 200 }) { cout << "Mahjong_B" << endl; }
Mahjong_B::~Mahjong_B() {}