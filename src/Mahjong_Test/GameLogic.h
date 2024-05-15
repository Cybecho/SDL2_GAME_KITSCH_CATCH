#pragma once

#include "Include.h"
#include "obj_Mahjong_Base.h"
#include "obj_Mahjong_Derived.h"
#include "obj_VFX.h"

class Mahjong; // 컴파일 시간 단축을 위해 전방 선언을 추가합니다.

//! 필수 함수
void InitGame();
void HandleEvents();
void Update();
void Render();
void ClearGame();

//! 사용자 정의 함수
void LoadMahjongBlocksFromCSV(int level, int seed, int numDims);
void vector2stack(std::vector<std::unique_ptr<Mahjong>>::iterator it);
void createBonk(int x, int y); // 추가된 함수 선언

//! 업데이트 함수
void LoadMahjongBlocksIfEmpty();
void RemoveSameTypeBlocks();
void AlignStackBlocks();
void UpdateVectorBlocks();
void UpdateStackBlocks();
void UpdateBonks();
void UpdateScore(int score);

//! 정렬 함수
bool compareBlocks(const std::unique_ptr<Mahjong>& a, const std::unique_ptr<Mahjong>& b);
void quickSort(std::vector<std::unique_ptr<Mahjong>>& blocks, int left, int right);
void sortPairedBlocks();


//! 전역 변수
extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern Uint32 g_frame_per_sec;
extern int g_blockCount;
