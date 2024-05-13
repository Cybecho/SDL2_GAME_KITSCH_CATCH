#pragma once

#include "Include.h"

void InitGame();
void HandleEvents();
void Update();
void Render();
void ClearGame();
void LoadMahjongBlocksFromCSV(int level, int seed, int numDims);

extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern Uint32 g_frame_per_sec;
extern int g_blockCount;