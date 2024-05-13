#include "obj_Mahjong_Base.h"

Mix_Chunk* Mahjong::m_sound = nullptr;
SDL_Texture* Mahjong::m_texture = nullptr;

Mahjong::Mahjong(int x, int y, SDL_Renderer* renderer, const SDL_Rect& sourceRect)
    : m_x(x), m_y(y), clicked(false), m_sourceRect(sourceRect), m_blockSize(BLOCK_SIZE), m_blockScale(BLOCK_SCALE), clickEnable(true),
    m_originalX(x), m_originalY(y), m_shakeTimer(0), m_isShaking(false) {
    if (!m_texture) {
        loadTexture(renderer);
    }

    if (!m_sound) {
        Set2Sound();
    }
    //! 생성자 멘트
    // cout << "Mahjong Create" << " x : " << this->m_x << " y : " << this->m_y << endl;
}

void Mahjong::update() {
    if (clicked) {
		// 추후에 구현
	}
    if (m_isShaking) {
        m_shakeTimer++;
        if (m_shakeTimer >= 30) { // 30 프레임 동안 떨림 애니메이션 재생
            m_x = m_originalX;
            m_y = m_originalY;
            m_isShaking = false;
            m_shakeTimer = 0;
        }
    }
}

void Mahjong::handleClick() {
    if (clicked) 
    {
        // 추후에 구현
    }
}

bool Mahjong::isClicked(int x, int y) const {
    return x >= m_x && x < m_x + (m_blockSize / m_blockScale) && y >= m_y && y < m_y + (m_blockSize / m_blockScale);
}

void Mahjong::Set2Sound() {
    if (!m_sound) {
        m_sound = Mix_LoadWAV("../Resources/ClickBlock.mp3");
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

void Mahjong::shakeBlocks() {
    if (!m_isShaking) {
        m_isShaking = true;
        m_shakeTimer = 0;
    }

    if (m_isShaking) {
        int shakeRange = 5;
        int offsetX = rand() % (shakeRange * 2 + 1) - shakeRange;
        int offsetY = rand() % (shakeRange * 2 + 1) - shakeRange;
        m_x = m_originalX + offsetX;
        m_y = m_originalY + offsetY;
    }
}

void Mahjong::render(SDL_Renderer* renderer) const {
    SDL_Rect dstRect = { m_x, m_y, static_cast<int>(m_blockSize / m_blockScale), static_cast<int>(m_blockSize / m_blockScale) };

    if (clickEnable) { //! 클릭 가능한 경우
        SDL_RenderCopy(renderer, m_texture, &m_sourceRect, &dstRect);
    }
    else { //! 클릭 불가능한 경우
        Uint8 alpha = 128; // 반투명도 설정 (0-255)
        SDL_SetTextureAlphaMod(m_texture, alpha); // 투명도 설정
        SDL_RenderCopy(renderer, m_texture, &m_sourceRect, &dstRect);
        SDL_SetTextureAlphaMod(m_texture, 255); // 원래 투명도로 복구
    }
}

void Mahjong::destroyTexture() {
    SDL_DestroyTexture(m_texture);
    m_texture = nullptr;
}

void Mahjong::loadTexture(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("../Resources/Mahjong.png");
    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}