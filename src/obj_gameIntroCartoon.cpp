#include "obj_gameIntroCartoon.h"

Cartoon::Cartoon() {
	surface = 0;
	texture = 0;
	source_rect = { 0, };
	destination_rect = { 0, };
}

Cartoon::~Cartoon() {
}

void Cartoon::FadeIn(double seconds) {
	fade_delta = 255. / (seconds * 33.);
	if (fade_value + fade_delta < 255) {
		fade_value = fade_value + fade_delta;
		fade_alpha = (int)fade_value;
	}
	else {
		fade_alpha = 255;
		is_fade_finished = true;
	}
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); // Required to apply ALPHA VALUE
	SDL_SetTextureAlphaMod(texture, fade_alpha);
	SDL_RenderCopy(g_renderer, texture, &source_rect, &destination_rect);
}

void Cartoon::Delay(double seconds) {
	if (delay_elapsed_time < seconds * 33) {
		delay_elapsed_time++;
	}
	else {
		is_delay_finished = true;
	}
}
