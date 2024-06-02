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
	void loadTxts();
	void renderScore();
	void renderButtons();
	void renderCat();

	void MouseButtonEvents(); //all mouse button events
	void updateTimer();
	

	//! ************************** getter&setter **************************
	int getM_RandCat() const { return m_RandCat; }
	void setM_RandCat(int a) { m_RandCat = a; }


	SDL_Event event;

	bool isRule;


protected:
	int m_RandCat;//0: 왼쪽걷기 , 1: 오른쪽걷기, 2 : 앉기, 3 : 자기

	SDL_Texture* main_bg;
	SDL_Texture* cat;
	SDL_Texture* inter_bt;
	SDL_Texture* play_bt;
	SDL_Texture* setting_bt;
	SDL_Texture* setting;

	SDL_Texture* cat_sit;
	SDL_Texture* cat_sleep;
	//SDL_Texture* cat_walk_left;
	//SDL_Texture* cat_walk_right;

	SDL_Texture* exp_txt; //"EXP" text
	TTF_Font* score_font; //get score from txt file
	SDL_Texture* score_txt;
	
	SDL_Texture* rule;

	SDL_Rect playBT_rect;
	SDL_Rect playBT_dir_rect;
	SDL_Rect cat_rect;
	SDL_Rect sleepBT_rect;
	SDL_Rect sitBT_rect;
	SDL_Rect settingBT_rect;
	SDL_Rect ques_rect;
	SDL_Rect rule_rect;
	SDL_Rect rule_Xkey_rect;
	SDL_Rect cat_main_rect;


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
extern bool isBasicCat;


extern int count_;
extern int sec; //play second
extern int last_sec;
extern int org_score_int;

extern int sprite_num; //고양이 스프라이트 x좌표

extern string original_score;
extern SDL_Rect timebar_rect;
