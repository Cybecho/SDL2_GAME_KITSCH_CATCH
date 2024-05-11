#pragma once
#include "Include.h"

// 가상 부모 클래스
class bonk{
public:
    //~ 생성자와 소멸자
    bonk();
    bonk(int x, int y, SDL_Renderer* renderer);  
    ~bonk();

    //~ 초기화 함수
    void InitGmae();
    void ClearGame();

    //~ SDL전용 함수
    void HandleEvents();
    void Update();
    void Render(SDL_Renderer* renderer) const;

    //~ 멤버 함수
    static void destroyTexture();
    static void loadTexture(SDL_Renderer* renderer);
    static void Set2Sound();
    static void Clear2Sound();

private:
    int mX;
    int mY;
    int m_frame;
    int m_frameCount;
    int m_frameDelay;
    int m_frameTimer;

    //~ 메모리 절약을 위해 재사용되는 텍스쳐와 사운드는 정적 변수로 선언
    static SDL_Texture* m_texture;
    SDL_Rect m_sourceRects[3];
    static Mix_Chunk* m_sound;
};