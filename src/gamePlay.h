#pragma once
#include "gameClass.h"
#include "gameLogic.h"
//play scene


class gamePlay : public gameClass {
public:
	
	//! ******************** 생성자 소멸자 ******************** 
	gamePlay();
	virtual ~gamePlay();
	
	//! ********************** 기본 함수 **********************
	void HandleEvents();
	void Update();
	void Render();
	void resetGame();

	//! ********************** 페이즈 전환 **********************
	void changePhase(GamePhase status);
	void gotoHome();

	//! ********************** 점수 및 타이머 **********************
	void resetTimer();
	void stageLimitTime();
	void updateScore(int s);
	void updateTimer();
	void checkQuit();
	void addSeconds(int seconds);
	void changeTimebar();

	//! ********************** 데이터 import 및 Rendering **********************
	void loadIMGs();
	void loadSounds();
	void renderCat();
	void renderSetting();

	//! ********************** 마우스 이벤트 **********************
	void MouseButtonEvents();

	//! ********************** 마우스 이벤트 **********************
	void writeScore(string s);

	//! ************************** gameLogic **************************
	void increaseLevelLogic();			//~ 레벨 증가 로직
	void loadMahjongBlocks();			//~ 맞춰야 할 블록 로드
	void checkAndLoadMahjongBlocks();	//~ 맞춰야 할 블록 체크 및 로드
	void checkGameStatus();				//~ 게임 상태 체크 (g_status 상태 확인)

	//! ************************** 고양이 상태 **************************
	void updateCatStatus();

	//! ************************** getter & setter **************************
	int getLastSec() const { return last_sec; }
	void setLastSec(int sec) { last_sec = sec; }

	

protected:
	SDL_Event event;
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

	SDL_Texture* cat_sit;
	SDL_Texture* cat_sleep;
	SDL_Texture* cat_walk_left;
	SDL_Texture* cat_walk_right;

	SDL_Rect playground_rect;
	SDL_Rect stack_rect;
	
	SDL_Rect cat_rect;
	SDL_Rect cat_rect2;
	SDL_Rect cat_play_rect;
	SDL_Rect settingBT_rect;

	//score
	SDL_Rect exp_rect;
	SDL_Rect score_rect;

	Mix_Chunk* setting_SoundEffect;

	int stage;
	int count_;
	
	int limit_sec;
	int last_sec; //last second (stage second)

};

extern Mix_Music* main_music;
extern Mix_Music* play_music;
extern Mix_Music* clear_music;
extern Mix_Music* gameover_music;
extern bool cat_status;
extern bool clear_reset;
extern bool gameover_reset;

extern Uint32 g_last_time_ms;
extern bool isChanged;
extern bool isForcedQuit;
extern bool isBasicCat; //고양이 기본 상태

extern SDL_Rect timebar_rect;
extern string score;

extern string original_score;
extern int org_score_int;
extern int plus_score_int; //load from txt file

extern int sprite_num; //고양이 스프라이트 x좌표
extern int RandI; //랜덤 초 
extern int p_RandCat; //random cat status
extern int sec;