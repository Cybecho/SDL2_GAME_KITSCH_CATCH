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
    void setN(int N) { m_N = N; }
    void setM(int M) { m_M = M; }
    void setR(int R) { m_R = R; }
    
    //! 하위 객체들의 정보를 받아올 가상함수 
    virtual string getType() const = 0;
    
    //! 흔들림 효과
    void shakeBlocks(int duration);

    //! 마우스가 올라갔을때 객체 호버링
    bool isHovered(int x, int y) const; 
    void setHovered(bool value) { hovered = value; }

    //! 클릭된 블록의 정보
    tuple<int, int, int> getClickedBlockInfo() const { return m_clickedBlockInfo; }

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
    int m_N; // ROW
    int m_M; // COL
    int m_R; // DIMENTION
    //! 호버링
    bool hovered;
    float m_hoverScale;
    //! 클릭된 블록의 정보
    tuple<int, int, int> m_clickedBlockInfo;
    //! 리소스 정보
    SDL_Rect m_sourceRect;
    static Mix_Chunk* m_sound;
    static SDL_Texture* m_texture;
};