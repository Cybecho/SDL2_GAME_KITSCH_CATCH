#pragma once

#include "Include.h"
#include "obj_Mahjong_Base.h"
#include "obj_Mahjong_Derived.h"
#include "obj_VFX.h"

class Mahjong; // 컴파일 시간 단축을 위해 전방 선언을 추가합니다.
extern vector<unique_ptr<Mahjong>> g_vector;

//! 필수 함수
void InitGame();
void HandleEvents();
void Update();
void Render();
void ClearGame();

//! 사용자 정의 함수
void LoadMahjongBlocksFromCSV(int level, int seed, int numDims);
void vector2stack(vector<unique_ptr<Mahjong>>::iterator it);
void createBonk(int x, int y); // 추가된 함수 선언

//! 업데이트 함수
void LoadMahjongBlocksIfEmpty(int level);
void RemoveSameTypeBlocks();
void AlignStackBlocks();
void UpdateVectorBlocks();
void UpdateStackBlocks();
void UpdateBonks();
void UpdateScore(int score);

//! Empty Block 체크 함수
bool checkEmptyBlocks();
int countEmptyBlocks();

//! 정렬 함수
bool compareBlocks(const unique_ptr<Mahjong>& a, const unique_ptr<Mahjong>& b);
void quickSort(vector<unique_ptr<Mahjong>>& blocks, int left, int right);
void sortPairedBlocks();

//! 파일 세기 함수
int countDir(const string& path);
int countFiles(const string& path);

//! 전역 변수
extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern Uint32 g_frame_per_sec;
extern int g_blockCount;
extern int g_level;
extern int g_status; // 0 : playing , 1 : gameclear, 2 : gameover
extern int MAX_LEVEL;

