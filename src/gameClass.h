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

void InitGame();
void ClearGame();

extern int g_current_game_phase;
extern bool g_flag_running;
extern Uint32 g_last_time_ms;
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;
