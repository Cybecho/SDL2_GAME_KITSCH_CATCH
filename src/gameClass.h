#pragma once

#include "Include.h"

class gameClass {
public:
	gameClass();
	~gameClass();

	virtual void HandleEvents() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect source_rect;
	SDL_Rect destination_rect;
};

void InitGame(); //Init game이랑 clear game은 virtual화 x, 클래스화x
void ClearGame();

// Game Phases
const int PHASE_INTRO = 0;
const int PHASE_ENDINGCLEAR = 1;
const int PHASE_ENDINGGAMEOVER = 2;
//const int PHASE_MAIN = 1;
//const int PHASE_PLAY = 2;
//const int PHASE_PAUSE = 5;

extern int g_current_game_phase;
extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;
extern const int g_window_margin;