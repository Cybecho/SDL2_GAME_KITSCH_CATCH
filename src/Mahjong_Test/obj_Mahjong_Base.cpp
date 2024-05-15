#include "obj_Mahjong_Base.h"

Mix_Chunk* Mahjong::m_sound = nullptr;
SDL_Texture* Mahjong::m_texture = nullptr;
<<<<<<< HEAD
extern vector<vector<vector<unique_ptr<Mahjong>>>> g_vector; // ¸¶ÀÛ ºí·Ï »ı¼º º¤ÅÍ
=======

//! Àü¿ª º¤ÅÍ ¼±¾ğ (½ÇÁ¦·Î´Â gameLogic ¿¡ ¾²ÀÓ)
//! cehckClickEnable() ÇÔ¼ö¿¡¼­ »ç¿ëÇÏ±â À§ÇØ extern ¼±¾ğ
extern vector<unique_ptr<Mahjong>> g_vector;
>>>>>>> parent of 7387946 (ì„ íƒ ì œí•œ ë¡œì§ êµ¬í˜„ 2 (ë¯¸ì™„))

Mahjong::Mahjong(int x, int y, SDL_Renderer* renderer, const SDL_Rect& sourceRect)
    : m_x(x), m_y(y), clicked(false), m_sourceRect(sourceRect), m_blockSize(BLOCK_SIZE), m_blockScale(BLOCK_SCALE), clickEnable(true),
    m_originalX(x), m_originalY(y), m_shakeDuration(0), m_shakeTimer(0), m_isShaking(false), hovered(false), m_hoverScale(1.1f),
    m_N(0), m_M(0), m_R(0) { // ÃÊ±â°ª ¼³Á¤
    if (!m_texture) {
        loadTexture(renderer);
    }

    if (!m_sound) {
        Set2Sound();
    }
    //! »ı¼ºÀÚ ¸àÆ®
    // cout << "Mahjong Create" << " x : " << this->m_x << " y : " << this->m_y << endl;
}

void Mahjong::update() {
    checkClickEnable();

    if (m_isShaking) {
        m_shakeTimer++;
        if (m_shakeTimer >= m_shakeDuration) {
            m_x = m_originalX;
            m_y = m_originalY;
            m_isShaking = false;
            m_shakeTimer = 0;
        }
        else {
            int shakeRange = 5;
            int offsetX = rand() % (shakeRange * 2 + 1) - shakeRange;
            int offsetY = rand() % (shakeRange * 2 + 1) - shakeRange;
            m_x = m_originalX + offsetX;
            m_y = m_originalY + offsetY;
        }
    }
}

void Mahjong::handleClick() {
    if (clicked) {
        clicked = false;
        //~ vector2stack¸¦ ±¸ÇöÇÏ±â À§ÇØ Å¬¸¯µÈ ºí·ÏÀÇ Á¤º¸¸¦ ÀúÀå
        m_clickedBlockInfo = make_tuple(m_N, m_M, m_R);
    }
}

bool Mahjong::isClicked(int x, int y) const {
    return x >= m_x && x < m_x + (m_blockSize / m_blockScale) && y >= m_y && y < m_y + (m_blockSize / m_blockScale);
}

void Mahjong::Set2Sound() {
    if (!m_sound) {
        m_sound = Mix_LoadWAV("../../res/ClickBlock.mp3");
        if (!m_sound) {
            printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        }
        else {
            Mix_VolumeChunk(m_sound, 32);
        }
    }
}

void Mahjong::Play2Sound() {
    if (m_sound) {
        Mix_PlayChannel(-1, m_sound, 0);
    }
}

void Mahjong::Clear2Sound() {
    if (m_sound) {
        Mix_FreeChunk(m_sound);
        m_sound = nullptr;
    }
}

void Mahjong::shakeBlocks(int duration) {
    if (!m_isShaking) {
        m_isShaking = true;
        m_shakeDuration = duration;
        m_shakeTimer = 0;
    }
}

bool Mahjong::isHovered(int x, int y) const {
    float scale = m_blockScale;
    int blockWidth = static_cast<int>(m_blockSize * scale);
    int blockHeight = static_cast<int>(m_blockSize * scale);
    int blockX = static_cast<int>(m_x - ((m_blockSize * scale) - m_blockSize) / 2);
    int blockY = static_cast<int>(m_y - ((m_blockSize * scale) - m_blockSize) / 2);

    // È£¹ö¸µ ¿µ¿ªÀ» ºí·Ï Å©±âÀÇ 80%·Î Á¶Á¤
    float hoverRatio = 0.8f;
    int hoverWidth = static_cast<int>(blockWidth * hoverRatio);
    int hoverHeight = static_cast<int>(blockHeight * hoverRatio);
    int hoverX = blockX + (blockWidth - hoverWidth) / 2;
    int hoverY = blockY + (blockHeight - hoverHeight) / 2;

    return x >= hoverX && x < hoverX + hoverWidth && y >= hoverY && y < hoverY + hoverHeight;
}

void Mahjong::checkClickEnable() {
    clickEnable = true;

<<<<<<< HEAD
    for (int dim = m_N + 1; dim < g_vector.size(); ++dim) {
        if (m_M >= g_vector[dim].size()) {
            continue;
        }
        if (m_R >= g_vector[dim][m_M].size()) {
            continue;
        }
        if (g_vector[dim][m_M][m_R].get()) {
            clickEnable = false;
            return;
=======
    int N = (m_y - PIVOT_Y) / BLOCK_SIZE;
    int M = (m_x - PIVOT_X) / (BLOCK_SIZE / 2);
    int R = (m_blockSize - BLOCK_SIZE) / (BLOCK_SIZE / 2);

    for (const auto& block : g_vector) {
        int blockN = (block->getY() - PIVOT_Y) / BLOCK_SIZE;
        int blockM = (block->getX() - PIVOT_X) / (BLOCK_SIZE / 2);
        int blockR = (block->m_blockSize - BLOCK_SIZE) / (BLOCK_SIZE / 2);

        if (blockR > R) {
            if ((blockN == N + 1 && blockM == M && blockR == R + 1) ||
                (blockN == N + 1 && blockM == M - 1 && blockR == R + 1) ||
                (blockN == N + 1 && blockM == M && blockR == R) ||
                (blockN == N + 1 && blockM == M - 1 && blockR == R)) {
                clickEnable = false;
                break;
            }
>>>>>>> parent of 7387946 (ì„ íƒ ì œí•œ ë¡œì§ êµ¬í˜„ 2 (ë¯¸ì™„))
        }
    }
}

void Mahjong::render(SDL_Renderer* renderer) const {
    //! ºí·ÏÀÇ Å©±â¸¦ È£¹ö¸µ ¿©ºÎ¿¡ µû¶ó »ïÇ× ¿¬»êÀÚ·Î ¼³Á¤
    //~ ¸¸¾à È£¹ö¸µ »óÅÂ¶ó¸é? °´Ã¼ Å©±â´Â m_hoverScale
    //~ ¾Æ´Ï¶ó¸é? °´Ã¼ Å©±â´Â m_blockScale
    float scale = hovered ? m_hoverScale : m_blockScale;

    SDL_Rect dstRect = {
        static_cast<int>(m_x - ((m_blockSize * scale) - m_blockSize) / 2),
        static_cast<int>(m_y - ((m_blockSize * scale) - m_blockSize) / 2),
        static_cast<int>(m_blockSize * scale),
        static_cast<int>(m_blockSize * scale)
    };

    //! Å¬¸¯ÀÌ °¡´ÉÇÑ »óÅÂ¶ó¸é? Å¬¸¯ °¡´ÉÇÑ »óÅÂ·Î ·»´õ¸µ (ºÒÅõ¸í)
    //! Å¬¸¯ÀÌ ºÒ°¡´ÉÇÑ »óÅÂ¶ó¸é? Å¬¸¯ ºÒ°¡´ÉÇÑ »óÅÂ·Î ·»´õ¸µ (¹İÅõ¸í)
    if (clickEnable) {
        SDL_RenderCopy(renderer, m_texture, &m_sourceRect, &dstRect);
    }
    else {
        Uint8 alpha = 128;
        Uint8 grayValue = 128;

        SDL_SetTextureAlphaMod(m_texture, alpha);
        SDL_SetTextureColorMod(m_texture, grayValue, grayValue, grayValue);
        SDL_RenderCopy(renderer, m_texture, &m_sourceRect, &dstRect);
        SDL_SetTextureAlphaMod(m_texture, 255);
        SDL_SetTextureColorMod(m_texture, 255, 255, 255);
    }
}

void Mahjong::destroyTexture() {
    SDL_DestroyTexture(m_texture);
    m_texture = nullptr;
}

void Mahjong::loadTexture(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("../../res/Mahjong.png");
    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}