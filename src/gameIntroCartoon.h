#pragma once

#include "include.h"
#include "gameIntro.h"

class Cartoon{
public:
	Cartoon();
	~Cartoon();

	void FadeIn(int seconds);
	void Delay(int seconds);

	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect source_rect;
	SDL_Rect destination_rect;

	// FadeIn() Variables
	int fade_alpha = 0;
	double fade_value = 0;
	double fade_delta = 0;
	bool is_fade_finished = false;

	// Delay() Variables
	int delay_elapsed_time = 0;
	bool is_delay_finished = false;
};