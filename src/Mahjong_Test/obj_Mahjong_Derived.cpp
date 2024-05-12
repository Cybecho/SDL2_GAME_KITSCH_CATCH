#include "obj_Mahjong_Derived.h"

//! Mahjong_A
Mahjong_A::Mahjong_A(int x, int y, SDL_Renderer* renderer) : Mahjong(x, y, renderer) {}
Mahjong_A::~Mahjong_A() {}

void Mahjong_A::loadTexture(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("../Resources/fire_A.png");
    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

//! Mahjong_B
Mahjong_B::Mahjong_B(int x, int y, SDL_Renderer* renderer) : Mahjong(x, y, renderer) {}
Mahjong_B::~Mahjong_B() {}

void Mahjong_B::loadTexture(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("../Resources/fire_B.png");
    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}