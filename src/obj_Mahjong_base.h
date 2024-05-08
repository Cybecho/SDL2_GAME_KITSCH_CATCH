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
    
    //~ 사운드 함수
    static void Set2Sound();
    static void Clear2Sound();

    //~ 초기화 함수
    void InitGmae();
    void ClearGame();

    //~ SDL전용 함수
    void HandleEvents();
    void Update();
    void Render(SDL_Renderer* renderer) const;

    

  private:
    int mX;
    int mY;
    int mScore;
    bool m_clickEnable;

    //~ 메모리 절약을 위해 재사용되는 텍스쳐와 사운드는 정적 변수로 선언
    static SDL_Texture* m_texture;
    SDL_Rect m_sourceRects[3];
    static Mix_Chunk* m_sound;
};