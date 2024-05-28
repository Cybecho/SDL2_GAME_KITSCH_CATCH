#pragma once
#include "include.h"
#include "gameClass.h"
#include "obj_gameIntroCartoon.h"
#define CLEAR_BG_IMG 2
#define CLEAR_CAT_IMG 6

class gameEndingClear : public gameClass {
public:
	gameEndingClear();
	~gameEndingClear();

	void HandleEvents();
	void Update();
	void Render();

	std::vector<Cartoon> bg; // ��� �̹��� (Fade�� ���� Cartoon ���)
	std::vector<Cartoon> cat; // ������ �̹��� (Fade�� ���� Cartoon ���)
	SDL_Rect mainBT_rect;
	SDL_Rect retryBT_rect;
	int cat_cut; // ������ �̹��� ��ȣ
	bool bt_clickable;

	Mix_Chunk* SoundEffect;
};
extern Mix_Music* clear_music;
extern Mix_Music* gameover_music;
extern Mix_Music* play_music;
extern bool gameover_reset;

extern bool isChanged;
extern SDL_Rect timebar_rect;
extern int sec;