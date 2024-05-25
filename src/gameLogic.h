#pragma once

#include "Include.h"
#include "obj_Mahjong_Base.h"
#include "obj_Mahjong_Derived.h"
#include "obj_VFX.h"

class gameLogic {
public:
    gameLogic();
    ~gameLogic();

    void InitGame();
    void HandleEvents();
    void Update();
    void Render();
    void ClearGame();
    void LoadMahjongBlocksFromCSV(int level, int seed, int numDims);
    void vector2stack(vector<unique_ptr<Mahjong>>::iterator it);
    void createBonk(int x, int y);
    void LoadMahjongBlocksIfEmpty(int level);
    void RemoveSameTypeBlocks();
    void AlignStackBlocks();
    void UpdateVectorBlocks();
    void UpdateStackBlocks();
    void UpdateBonks();
    void UpdateScore(int score);
    bool checkEmptyBlocks();
    int countEmptyBlocks();
    bool compareBlocks(const unique_ptr<Mahjong>& a, const unique_ptr<Mahjong>& b);
    void quickSort(vector<unique_ptr<Mahjong>>& blocks, int left, int right);
    void sortPairedBlocks();
    int countDir(const string& path);
    int countFiles(const string& path);
    vector<unique_ptr<Mahjong>>& getVector() { return g_vector; }

private:
    vector<unique_ptr<Mahjong>> g_vector; // 마작 블록 생성 벡터
    vector<unique_ptr<Mahjong>> g_stack; // 마작 블록 스택 벡터
    vector<bonk> g_bonks; // 효과 블록 생성 벡터

    Mix_Chunk* wave1_;
    Mix_Music* music1_;

    SDL_Texture* text_score_;
    SDL_Rect text_score_rect_;
    TTF_Font* game_font_;

    SDL_Rect g_bg_source_rect;
    SDL_Rect g_bg_destination_rect;

    int g_level;
    int MAX_LEVEL;
    int g_status;
};

gameLogic* GameLogicInstance();
Uint32 shakeBlocksCallback(Uint32 interval, void* param);