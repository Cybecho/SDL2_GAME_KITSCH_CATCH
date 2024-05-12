#include "obj_Mahjong_Derived.h"

//! Mahjong_A
Mahjong_A::Mahjong_A(int x, int y, SDL_Renderer* renderer)
    : Mahjong(x, y, renderer, { 0 * BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE }) { cout << "Mahjong_A" << endl; }
Mahjong_A::~Mahjong_A() {}

//! Mahjong_B
Mahjong_B::Mahjong_B(int x, int y, SDL_Renderer* renderer)
    : Mahjong(x, y, renderer, { 1 * BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE }) { cout << "Mahjong_B" << endl; }
Mahjong_B::~Mahjong_B() {}

//! Mahjong_C
Mahjong_C::Mahjong_C(int x, int y, SDL_Renderer* renderer)
	: Mahjong(x, y, renderer, { 2 * BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE }) { cout << "Mahjong_C" << endl; }
Mahjong_C::~Mahjong_C() {}

//! Mahjong_D
Mahjong_D::Mahjong_D(int x, int y, SDL_Renderer* renderer)
	: Mahjong(x, y, renderer, { 3 * BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE }) { cout << "Mahjong_D" << endl; }
Mahjong_D::~Mahjong_D() {}