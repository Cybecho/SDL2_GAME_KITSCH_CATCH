#pragma once

#include "Include.h"

class gameClass {
public:
	gameClass();
	virtual ~gameClass();
	//virtual ~gameClass() {}; // {} is implemented in gameClass.cpp

	virtual void HandleEvents() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect source_rect;
	SDL_Rect destination_rect;
};


class Timer {
public:
	Timer();
	~Timer();

	void setInterval(int interval); //주기 설정
	void start();
	void pause();
	void resume();
	bool done(); //시간 다 되었는지 조사

private:
	double interval;
	double startTicks;
	double pauseTicks;
};


void InitGame();
void ClearGame();

extern int g_current_game_phase;
extern bool g_flag_running;
extern Uint32 g_last_time_ms;
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;





