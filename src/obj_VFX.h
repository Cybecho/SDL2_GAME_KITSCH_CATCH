#pragma once
#include "Include.h"

// 가상 부모 클래스
class bonk{
public:
    bonk();
    bonk(int x, int y);  
    ~bonk();

    //~ 초기화 함수
    void InitGmae();
    void ClearGame();
    //~ SDL전용 함수
    void HandleEvents();
    void Update();
    void Render();

private:
    int mX;
    int mY;
};