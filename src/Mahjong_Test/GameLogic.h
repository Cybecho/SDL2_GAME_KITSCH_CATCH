#pragma once

#include "Include.h"
#include "obj_Mahjong_Base.h"
#include "obj_Mahjong_Derived.h"
#include "obj_VFX.h"

class Mahjong; // ������ �ð� ������ ���� ���� ������ �߰��մϴ�.

//! �ʼ� �Լ�
void InitGame();
void HandleEvents();
void Update();
void Render();
void ClearGame();

//! ����� ���� �Լ�
void LoadMahjongBlocksFromCSV(int level, int seed, int numDims);
void vector2stack(std::vector<std::unique_ptr<Mahjong>>::iterator it);
void createBonk(int mouse_x, int mouse_y); // �߰��� �Լ� ����

//! ������Ʈ �Լ�
void LoadMahjongBlocksIfEmpty();
void RemoveSameTypeBlocks();
void AlignStackBlocks();
void UpdateVectorBlocks();
void UpdateStackBlocks();
void UpdateBonks();
void UpdateScore(int score);

//! ���� ����
extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern Uint32 g_frame_per_sec;
extern int g_blockCount;
