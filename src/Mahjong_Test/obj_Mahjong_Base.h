#pragma once
#include "Include.h"
#include "obj_VFX.h"

class Mahjong {
public:
    //! 생성자 소멸자
    Mahjong(int x, int y, SDL_Renderer* renderer, const SDL_Rect& sourceRect);
    virtual ~Mahjong() {};

    //! 기본 함수들
    void update();
    void render(SDL_Renderer* renderer) const;
    static void destroyTexture();
    static void Set2Sound();
    static void Play2Sound();
    static void Clear2Sound();

    //! 클릭 가능한지
    bool isClickable() const { return clickEnable; }
    void setClickable(bool value) { clickEnable = value; }
    void setPosition(int n, int m, int r) { m_n = n; m_m = m; m_r = r; }
    int getN() const { return m_n; }
    int getM() const { return m_m; }
    int getR() const { return m_r; }
    void checkClickEnable();

    //! 클릭 되었을때
    bool isClicked(int x, int y) const;
    void setClicked(bool value) { clicked = value; }
    void handleClick();

    //! Getter & Setter
    int getScore() const { return m_score; }
    void setScore(int score) { m_score = score; }
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    void setX(int x) { m_x = x; m_originalX = x; }
    void setY(int y) { m_y = y; m_originalY = y; }

    //! 하위 객체들의 정보를 받아올 가상함수 
    virtual string getType() const = 0;

    //! 흔들림 효과
    void shakeBlocks(int duration);

    //! 마우스가 올라갔을때 객체 호버링
    bool isHovered(int x, int y) const;
    void setHovered(bool value) { hovered = value; }

protected:
    static void loadTexture(SDL_Renderer* renderer);
    //! 기본 멤버 변수
    int m_x;
    int m_y;
    int m_blockScale;
    int m_blockSize;
    int m_score = 100;
    //! 흔들림 효과
    int m_originalX;
    int m_originalY;
    int m_shakeTimer;
    int m_shakeDuration;
    bool m_isShaking;
    //! 클릭 가능 여부
    bool clickEnable;
    bool clicked;
    int m_n;
    int m_m;
    int m_r;
    //! 호버링
    bool hovered;
    float m_hoverScale;
    //! 리소스 정보
    SDL_Rect m_sourceRect;
    static Mix_Chunk* m_sound;
    static SDL_Texture* m_texture;
};