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




Timer::Timer() {
	interval = 0;
	startTicks = 0;
	pauseTicks = 0;
}

Timer::~Timer() {

}

void Timer::setInterval(int interval) {
	this->interval = interval;
}

void Timer::start() {
	startTicks = SDL_GetTicks();
}

void Timer::pause() {
	if (pauseTicks != 0) {
		startTicks += SDL_GetTicks() - pauseTicks;
		pauseTicks = 0;
	}
}

void Timer::resume() {
	if (pauseTicks != 0) {
		startTicks += SDL_GetTicks() - pauseTicks;
		pauseTicks = 0;
	}
}

bool Timer::done() {
	if (pauseTicks == 0 && SDL_GetTicks() - startTicks >= interval) {
		start();
		return true;
	}
	return false;
}

