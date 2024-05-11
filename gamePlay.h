#pragma once
#include "gameClass.h"

//play scene


class gamePlay : public gameClass {
public:
	gamePlay();
	~gamePlay();

	void changePhaseToEnding();
	void HandleEvents();
	void Update();
	void Render();

	string score;

	SDL_Texture* play_bg;
	SDL_Texture* playground_img;
	SDL_Texture* stack_img;
	SDL_Texture* timebar_bg;
	SDL_Texture* cat;
	SDL_Texture* setting_bt;

	SDL_Rect playground_rect;
	SDL_Rect stack_rect;
	SDL_Rect timebar_bg_rect;
	SDL_Rect timebar_bg_dir_rect;
	SDL_Rect timebar_rect;
	SDL_Rect cat_rect;
	SDL_Rect settingBT_rect;

	//score
	SDL_Rect exp_rect;
	SDL_Rect score_rect;
};

extern Mix_Music* play_music;
extern Mix_Music* clear_music;