#include "obj_gameIntroCartoon.h"

Cartoon::Cartoon() {
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
	SDL_SetTextureBlendMode(imgClass.texture, SDL_BLENDMODE_BLEND); // Required to apply ALPHA VALUE
	SDL_SetTextureAlphaMod(imgClass.texture, fade_alpha);
	SDL_RenderCopy(g_renderer, imgClass.texture, &imgClass.srcrect, &imgClass.dstrect);
}

void Cartoon::CrossFade(ImgClass nextImgClass, double seconds) {
	crossfade_delta = 255. / (seconds * 33.);
	if (crossfade_value2 + crossfade_delta < 255) {
		crossfade_value1 = crossfade_value1 - crossfade_delta;
		crossfade_value2 = crossfade_value2 + crossfade_delta;
		crossfade_alpha1 = (int)crossfade_value1;
		crossfade_alpha2 = (int)crossfade_value2;
	}
	else {
		crossfade_alpha1 = 0;
		crossfade_alpha2 = 255;
		is_crossfade_finished = true;
	}
	SDL_SetTextureBlendMode(imgClass.texture, SDL_BLENDMODE_BLEND); // Required to apply ALPHA VALUE
	SDL_SetTextureBlendMode(nextImgClass.texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(imgClass.texture, crossfade_alpha1);
	SDL_SetTextureAlphaMod(nextImgClass.texture, crossfade_alpha2);
	SDL_RenderCopy(g_renderer, imgClass.texture, &imgClass.srcrect, &imgClass.dstrect);
	SDL_RenderCopy(g_renderer, nextImgClass.texture, &nextImgClass.srcrect, &nextImgClass.dstrect);
}

void Cartoon::Delay(double seconds) {
	if (delay_elapsed_time < seconds * 33) {
		delay_elapsed_time++;
	}
	else {
		is_delay_finished = true;
	}
}

void Cartoon::InitFade() {
	fade_alpha = fade_value = fade_delta = is_fade_finished = 0;
}

void Cartoon::InitCrossFade() {
	crossfade_alpha1 = crossfade_value2 = crossfade_delta = is_crossfade_finished = 0;
	crossfade_alpha2 = crossfade_value1 = 255;
}

void Cartoon::InitDelay() {
	delay_elapsed_time = is_delay_finished = 0;
}