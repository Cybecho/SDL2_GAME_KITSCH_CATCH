#include "obj_Mahjong_Base.h"

Mix_Chunk* Mahjong::m_sound = nullptr;
SDL_Texture* Mahjong::m_texture = nullptr;
extern vector<vector<vector<unique_ptr<Mahjong>>>> g_vector; // 마작 블록 생성 벡터

Mahjong::Mahjong(int x, int y, SDL_Renderer* renderer, const SDL_Rect& sourceRect)
    : m_x(x), m_y(y), clicked(false), m_sourceRect(sourceRect), m_blockSize(BLOCK_SIZE), m_blockScale(BLOCK_SCALE), clickEnable(true),
    m_originalX(x), m_originalY(y), m_shakeDuration(0), m_shakeTimer(0), m_isShaking(false), hovered(false), m_hoverScale(1.1f),
    m_N(0), m_M(0), m_R(0) { // 초기값 설정
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
        //~ vector2stack를 구현하기 위해 클릭된 블록의 정보를 저장
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

    // 호버링 영역을 블록 크기의 80%로 조정
    float hoverRatio = 0.8f;
    int hoverWidth = static_cast<int>(blockWidth * hoverRatio);
    int hoverHeight = static_cast<int>(blockHeight * hoverRatio);
    int hoverX = blockX + (blockWidth - hoverWidth) / 2;
    int hoverY = blockY + (blockHeight - hoverHeight) / 2;

    return x >= hoverX && x < hoverX + hoverWidth && y >= hoverY && y < hoverY + hoverHeight;
}

void Mahjong::checkClickEnable() {
    clickEnable = true;

    for (int dim = 0; dim < m_N; ++dim) {
        if (m_M >= g_vector[dim].size()) {
            continue;
        }
        if (m_R >= g_vector[dim][m_M].size()) {
            continue;
        }
        for (int col = m_R + 1; col < g_vector[dim][m_M].size(); ++col) {
            if (g_vector[dim][m_M][col].get()) {
                clickEnable = false;
                return;
            }
        }
        for (int row = m_M + 1; row < g_vector[dim].size(); ++row) {
            for (int col = 0; col < g_vector[dim][row].size(); ++col) {
                if (g_vector[dim][row][col].get()) {
                    clickEnable = false;
                    return;
                }
            }
        }
    }
}

void Mahjong::render(SDL_Renderer* renderer) const {
    //! 블록의 크기를 호버링 여부에 따라 삼항 연산자로 설정
    //~ 만약 호버링 상태라면? 객체 크기는 m_hoverScale
    //~ 아니라면? 객체 크기는 m_blockScale
    float scale = hovered ? m_hoverScale : m_blockScale;

    SDL_Rect dstRect = {
        static_cast<int>(m_x - ((m_blockSize * scale) - m_blockSize) / 2),
        static_cast<int>(m_y - ((m_blockSize * scale) - m_blockSize) / 2),
        static_cast<int>(m_blockSize * scale),
        static_cast<int>(m_blockSize * scale)
    };

    //! 클릭이 가능한 상태라면? 클릭 가능한 상태로 렌더링 (불투명)
    //! 클릭이 불가능한 상태라면? 클릭 불가능한 상태로 렌더링 (반투명)
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