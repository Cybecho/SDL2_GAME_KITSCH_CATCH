//메인화면
#pragma once
#include "gameClass.h"




class gameMain : public gameClass {
public:
	gameMain();
	virtual ~gameMain();
	void HandleEvents();
	void Update();
	void Render();
	
	void changePhaseToPlay();
	void loadImgs(); //load and init images
	void loadSounds();
	void renderScore();
	void renderButtons();

	void MouseButtonEvents(); //all mouse button events
	


	SDL_Event event;

	bool isRule;


protected:
	SDL_Texture* main_bg;
	SDL_Texture* cat;
	SDL_Texture* inter_bt;
	SDL_Texture* play_bt;
	SDL_Texture* setting_bt;
	SDL_Texture* setting;
	
	SDL_Texture* rule;

	SDL_Rect playBT_rect;
	SDL_Rect playBT_dir_rect;
	SDL_Rect cat_rect;
	SDL_Rect interBT_rect;
	SDL_Rect settingBT_rect;
	SDL_Rect ques_rect;
	SDL_Rect rule_rect;
	SDL_Rect rule_Xkey_rect;
	

	//text
	SDL_Rect exp_rect;
	SDL_Rect score_rect;

	Mix_Chunk* SoundEffect;

	Uint32 play_time_ms;
	
};
extern Mix_Music* main_music;
extern Mix_Music* play_music;
extern SDL_Surface* sprite;
extern bool cat_status; //false: image1, true : image2
extern bool isChanged;
extern bool isForcedQuit;


extern int count_;
extern int sec; //play second
extern int last_sec;
extern int org_score_int;

extern string original_score;
extern SDL_Rect timebar_rect;
