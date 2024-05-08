#pragma once

#include "Include.h"

class gameClass {
public:
	gameClass();
	virtual ~gameClass() {};

	virtual void HandleEvents() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect source_rect;
	SDL_Rect destination_rect;
};

void InitGame(); //Init gameÀÌ¶û clear gameÀº virtualÈ­ x, Å¬·¡½ºÈ­x
void ClearGame();

// Game Phases

extern int g_current_game_phase;
extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;
extern const int g_window_margin;
