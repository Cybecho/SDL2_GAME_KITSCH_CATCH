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

	//! ********************** 타이머 관련 **********************
	void resetTimer();
	void stageLimitTime();
	void updateTimer();
	void changeTimebar();
	void checkQuit();
	
	//! ********************** 점수 관련 **********************
	void updateScore(int s);
	void addSeconds(int seconds);
	void updateAddScore();
	void writeScore(const string& s);

	//! ********************** 데이터 import 및 Rendering **********************
	void loadIMGs();
	void loadSounds();
	void loadTxts();
	void renderCat();
	void renderSetting();

	//! ********************** 마우스 이벤트 **********************
	void MouseButtonEvents();
	

	//! ************************** gameLogic **************************
	void increaseLevelLogic();			//~ 레벨 증가 로직
	void loadMahjongBlocks();			//~ 맞춰야 할 블록 로드
	void checkAndLoadMahjongBlocks();	//~ 맞춰야 할 블록 체크 및 로드
	void checkGameStatus();				//~ 게임 상태 체크 (g_status 상태 확인)

	//! ************************** 고양이 상태 **************************
	void changeCatAnimation();
	void waterAnimation();
	void sleepAnimation();
	void fishAnimation();
	void mouseAnimation();


	//! ************************** getter & setter **************************
	int getLastSec() const { return last_sec; }
	int getLimitSec() const { return limit_sec; }
	int getAddSec() const { return add_sec; }
	int getAddScore() const { return addScore; }

	void setLastSec(int sec) { last_sec = sec; }
	void setLimitSec(int sec) { limit_sec = sec; }
	void setAddSec(int sec) { add_sec = sec; }
	void setAddScore(int score) { addScore = score; }
	

protected:
	SDL_Event event;
	gameLogic m_gameLogic; //~ 게임 로직 클래스 생성

	//const Uint32 p_start_time_ms;
	Uint32 p_last_time_ms;
	string string_time;
	string update_score; //누적 점수
	//int update_score; //new score
	//float timebarw;

	SDL_Texture* play_bg;
	SDL_Texture* cat;
	SDL_Texture* cat2;
	SDL_Texture* setting_bt;
	SDL_Texture* setting;
	SDL_Texture* score_text2; //get score from txt file

	TTF_Font* score_font;
	
	SDL_Texture* cat_sleep;
	SDL_Texture* cat_walk_left;
	SDL_Texture* cat_walk_right;
	SDL_Texture* cat_mouse;
	SDL_Texture* cat_fish;
	SDL_Texture* cat_scratcher;
	SDL_Texture* cat_water;

	SDL_Rect playground_rect;
	SDL_Rect stack_rect;
	
	SDL_Rect cat_rect;
	SDL_Rect cat_rect2;
	SDL_Rect cat_play_rect; //스프라이트 그림 4개일때
	SDL_Rect cat_play_rect2; //스프라이트 그림 3개일때
	SDL_Rect settingBT_rect;

	//score
	SDL_Rect exp_rect;
	SDL_Rect score_rect;

	Mix_Chunk* setting_SoundEffect;

	int stage;
	int count_;
	int Cat_Ani_Status; //cat animation status // 0 : 목마름, 1 : 배부름,  2 : 놀아주기,  3 : 잠자기
	
	int add_sec; //추가 시간
	int limit_sec; //제한 시간
	int last_sec; //last second (stage second)
	Uint32 lastMatchTime; // 추가 점수 확인용
	int addScore;	// 추가 점수 멤버 변수
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

///  점수 관련
extern string score;
extern string original_score;
extern int org_score_int; // 게임 시작 전 메모장에 저장된 점수
extern int total_Score_int; // 누적 총 점수(메인화면)

/// 
extern int sprite_num; //고양이 스프라이트 x좌표
extern int sec;