#pragma once

#include "Include.h"

class Mahjong {
public:
    Mahjong(int x, int y);
    virtual ~Mahjong();
    virtual void update() = 0;
    virtual void render(SDL_Renderer* renderer) const = 0;
    virtual bool isOutOfScreen() = 0;

protected:
    int m_x;
    int m_y;
};