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

	ImgClass imgClass;

	// FadeIn() Variables
	int fade_alpha = 0;
	double fade_value = 0;
	double fade_delta = 0;
	bool is_fade_finished = false;

	// CrossFade() Variables
	int crossfade_alpha1 = 0;
	int crossfade_alpha2 = 255;
	double crossfade_value1 = 255;
	double crossfade_value2 = 0;
	double crossfade_delta = 0;
	bool is_crossfade_finished = false;

	// Delay() Variables
	double delay_elapsed_time = 0;
	bool is_delay_finished = false;
};