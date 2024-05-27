#pragma once
#include "gameClass.h"
#include "gameLogic.h"
//play scene




class gamePlay : public gameClass {
public:
	
	gamePlay();
	virtual ~gamePlay();
	

	
	void play_timer(int interval);
	void changePhaseToEnding();
	void changePhaseToMain();
	void HandleEvents();
	void Update();
	void Render();
	void updateScore(int s);

	//! ************************** gameLogic **************************
	void increaseLevelLogic();			//~ 레벨 증가 로직
	void loadMahjongBlocks();			//~ 맞춰야 할 블록 로드
	void checkAndLoadMahjongBlocks();	//~ 맞춰야 할 블록 체크 및 로드
	void checkGameStatus();				//~ 게임 상태 체크 (g_status 상태 확인)

protected:

	gameLogic m_gameLogic; //~ 게임 로직 클래스 생성

	//const Uint32 p_start_time_ms;
	Uint32 p_last_time_ms;
	string string_time;
	string update_score;
	//int update_score; //new score
	//float timebarw;

	SDL_Texture* play_bg;
	SDL_Texture* playground_img;
	SDL_Texture* stack_img;
	SDL_Texture* timebar_bg;
	SDL_Texture* cat;
	SDL_Texture* cat2;
	SDL_Texture* setting_bt;
	SDL_Texture* setting;
	SDL_Texture* score_text2; //get score from txt file

	SDL_Rect playground_rect;
	SDL_Rect stack_rect;
	
	SDL_Rect cat_rect;
	SDL_Rect cat_rect2;
	SDL_Rect settingBT_rect;

	//score
	SDL_Rect exp_rect;
	SDL_Rect score_rect;

	Mix_Chunk* setting_SoundEffect;
};

extern Mix_Music* main_music;
extern Mix_Music* play_music;
extern Mix_Music* clear_music;
extern bool cat_status;
extern bool clear_reset;

extern Uint32 g_last_time_ms;
extern bool isChanged;
extern bool isForcedQuit;

extern SDL_Rect timebar_rect;
extern int stage;
extern int count_;
extern int sec; //play second
extern int limit_sec;
extern int last_sec; //last second (stage second)

extern string original_score;
extern int org_score_int;
extern int plus_score_int; //load from txt file