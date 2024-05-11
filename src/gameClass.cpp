#include "gameClass.h"

gameClass::gameClass() {
	surface = 0;
	texture = 0;
	source_rect = { 0, };
	destination_rect = { 0, };
}

gameClass::~gameClass() {
}

void InitGame() {
	g_flag_running = true;
	g_last_time_ms = 0;
}

void ClearGame() {
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
}
