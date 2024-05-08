#pragma once
#include "Include.h"
#include "obj_VFX.h"

// 가상 부모 클래스
class Mahjong{
  public:
    //~ 생성자
    virtual Majong() = 0;
    virtual ~Majong() = 0;
    
    //~ 부모함수에서 구현할 함수 & 공통 외부 함수
    void InitGmae();
    void ClearGame();
    void bonkVFX(); // 펑 터지는 이펙트 함수
    void swap2stack(); // 클릭되었을때 stack으로 이동
    void getPos(); // 단순 위치 반환 함수

    //~ SDL전용 함수
    void HandleEvents();
    void Update();
    void Render();

  private:
    int mX;
    int mY;
    int mScore;
    bool m_clickEnable;
};