//메인화면
#pragma once
#include "gameClass.h"




class MainScreen : public gameClass {
public:
	MainScreen();
	~MainScreen();

	void HandleEvents();
	void Update();
	void Render();

};
/*
int score;

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
SDL_Texture* exp_text; //"경험치" 글씨
SDL_Texture* score_text; //점수(경험치)
SDL_Rect exp_rect;
SDL_Rect score_rect;

*/
