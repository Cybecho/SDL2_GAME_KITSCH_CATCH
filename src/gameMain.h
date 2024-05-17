//메인화면
#pragma once
#include "gameClass.h"




class gameMain : public gameClass {
public:
	gameMain();
	virtual ~gameMain();
	void changePhaseToPlay();

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
	SDL_Rect playBT_dir_rect;
	SDL_Rect cat_rect;
	SDL_Rect interBT_rect;
	SDL_Rect settingBT_rect;

	//text
	SDL_Rect exp_rect;
	SDL_Rect score_rect;

	Mix_Chunk* SoundEffect;

	
	
};
extern Mix_Music* main_music;
extern Mix_Music* play_music;
extern SDL_Surface* sprite;
extern bool cat_status; //false: image1, true : image2
