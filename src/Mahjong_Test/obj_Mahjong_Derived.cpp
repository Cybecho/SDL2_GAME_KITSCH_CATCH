#include "obj_Mahjong_Derived.h"

//! Mahjong_A
Mahjong_A::Mahjong_A(int x, int y, SDL_Renderer* renderer)
    : Mahjong(x, y, renderer, { 0 * BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE }) {
    cout << "A ";
}
Mahjong_A::~Mahjong_A() { cout << "delete A" << endl; }

//! Mahjong_B
Mahjong_B::Mahjong_B(int x, int y, SDL_Renderer* renderer)
    : Mahjong(x, y, renderer, { 1 * BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE }) {
    cout << "B ";
}
Mahjong_B::~Mahjong_B() { cout << "delete B" << endl; }

//! Mahjong_C
Mahjong_C::Mahjong_C(int x, int y, SDL_Renderer* renderer)
    : Mahjong(x, y, renderer, { 2 * BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE }) {
    cout << "C ";
}
Mahjong_C::~Mahjong_C() { cout << "delete C" << endl; }

//! Mahjong_D
Mahjong_D::Mahjong_D(int x, int y, SDL_Renderer* renderer)
    : Mahjong(x, y, renderer, { 3 * BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE }) {
    cout << "D ";
}
Mahjong_D::~Mahjong_D() { cout << "delete D" << endl; }