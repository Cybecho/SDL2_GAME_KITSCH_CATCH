#pragma once

#include "Include.h"
#include "obj_Mahjong_Base.h"
#include "obj_Mahjong_Derived.h"
#include "obj_VFX.h"

class Mahjong; // 컴파일 시간을 단축하기 위해 전방 선언

//! 필수 함수
void InitGame();
void HandleEvents();
void Update();
void Render();
void ClearGame();

//! 사용자 정의 함수
void LoadMahjongBlocksFromCSV(int level, int seed, int numDims);
void vector2stack(std::vector<std::unique_ptr<Mahjong>>::iterator it); // 수정된 함수 선언

//! 전역 변수
extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern Uint32 g_frame_per_sec;
extern int g_blockCount;