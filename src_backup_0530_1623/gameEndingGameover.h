#pragma once
#include "include.h"
#include "gameClass.h"
#include "obj_gameIntroCartoon.h"
#define GAMEOVER_BG_IMG 2
#define GAMEOVER_CAT_IMG 5

class gameEndingGameover : public gameClass {
public:
	gameEndingGameover();
	~gameEndingGameover();

	void HandleEvents();
	void Update();
	void Render();

	vector<Cartoon> bg; // ��� �̹��� (Fade�� ���� Cartoon ���)
	vector<Cartoon> cat; // ������ �̹��� (Fade�� ���� Cartoon ���)
	SDL_Rect mainBT_rect;
	SDL_Rect retryBT_rect;
	int cat_cut; // ������ �̹��� ��ȣ
	bool bt_clickable;

	Mix_Chunk* SoundEffect;
};
extern Mix_Music* gameover_music;
extern Mix_Music* intro_music;
extern Mix_Music* play_music;
extern Mix_Music* main_music;
extern bool intro_reset;
extern bool isChanged;
extern SDL_Rect timebar_rect;
extern int sec;