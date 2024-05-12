#pragma once
#include "Include.h"
#include "obj_VFX.h"


class Mahjong {
public:
    Mahjong(int x, int y, SDL_Renderer* renderer, const SDL_Rect& sourceRect);
    virtual ~Mahjong() {};
    void update();
    void render(SDL_Renderer* renderer) const;
    static void destroyTexture();
    static void Set2Sound();
    static void Play2Sound();
    static void Clear2Sound();

    //! Ŭ�� ��������
    bool isClickable() const { return clickEnable; }
    void setClickable(bool value) { clickEnable = value; }
    //! Ŭ�� �Ǿ�����
    bool isClicked(int x, int y) const;
    void setClicked(bool value) { clicked = value; }
    void handleClick();

protected:
    static void loadTexture(SDL_Renderer* renderer);
    int m_x;
    int m_y;
    int m_blockScale;
    int m_blockSize;
    bool clickEnable = true; //! Drawing�ӽñ⿡�� setClickable�� �������ּ���
    bool clicked;
    SDL_Rect m_sourceRect;
    static Mix_Chunk* m_sound;
    static SDL_Texture* m_texture;
};