//메인화면
#pragma once
#include "gameClass.h"




class gameMain : public gameClass {
public:
	gameMain();
	~gameMain();

	void HandleEvents();
	void Update();
	void Render();

	string score;
	SDL_Texture* main_bg;
	SDL_Texture* cat;
	SDL_Texture* inter_bt;
	SDL_Texture* play_bt;
	SDL_Texture* setting_bt;

	SDL_Rect playBT_rect;
	SDL_Rect cat_rect;
	SDL_Rect interBT_rect;
	SDL_Rect settingBT_rect;

	//text
	SDL_Rect exp_rect;
	SDL_Rect score_rect;

	
};
extern Mix_Music* main_music;
