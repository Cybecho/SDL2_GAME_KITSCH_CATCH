#pragma once
#include "Include.h"

// 가상 부모 클래스
class Mahjong : public blockType_1{
  public:
    //~ 생성자
    blockType_1();
    blockType_1();

    //~ 멤버 함수
    static void destroyTexture();
    static void loadTexture(SDL_Renderer* renderer);

    //~ SDL전용 함수
    void 
  private:
    int mX;
    int mY;
    int mScore;
    bool m_clickEnable;

    
};