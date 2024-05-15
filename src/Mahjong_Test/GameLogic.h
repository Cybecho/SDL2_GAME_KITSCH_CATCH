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
void createBonk(int x, int y); // �߰��� �Լ� ����

//! ������Ʈ �Լ�
void LoadMahjongBlocksIfEmpty();
void RemoveSameTypeBlocks();
void AlignStackBlocks();
void UpdateVectorBlocks();
void UpdateStackBlocks();
void UpdateBonks();
void UpdateScore(int score);

//! Empty Block üũ �Լ�
bool checkEmptyBlocks();
int countEmptyBlocks();

//! ���� �Լ�
bool compareBlocks(const std::unique_ptr<Mahjong>& a, const std::unique_ptr<Mahjong>& b);
void quickSort(std::vector<std::unique_ptr<Mahjong>>& blocks, int left, int right);
void sortPairedBlocks();

//! ���� ���� �Լ�
int countDir(const std::string& path);
int countFiles(const std::string& path);

//! ���� ����
extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern Uint32 g_frame_per_sec;
extern int g_blockCount;

