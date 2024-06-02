#pragma once
#include "Include.h"
#include "gameIntro.h"

class Cartoon{
public:
	Cartoon();
	~Cartoon();

	void FadeIn(double seconds);
	void CrossFade(ImgClass nextImgClass, double seconds);
	void Delay(double seconds);
	void InitFade();
	void InitCrossFade();
	void InitDelay();

	ImgClass GetImgClass();
	void SetImgClassSurface(SDL_Surface* new_surface);
	void SetImgClassTexture(SDL_Texture* new_texture);
	void SetImgClassSrcRect(SDL_Rect new_srcrect);
	void SetImgClassDstRect(SDL_Rect new_dstrect);

	bool isFadeFinished() { return is_fade_finished; }
	bool isCrossFadeFinished() { return is_crossfade_finished; }
	bool isDelayFinished() { return is_delay_finished; }

private:
	ImgClass img_class;

	// FadeIn() Variables
	double fade_delta;
	double fade_value;
	int fade_alpha;
	bool is_fade_finished;

	// CrossFade() Variables
	double crossfade_delta;
	double crossfade_value;
	int crossfade_alpha1;
	int crossfade_alpha2;
	bool is_crossfade_finished;

	// Delay() Variables
	double delay_elapsed_time;
	bool is_delay_finished;
};