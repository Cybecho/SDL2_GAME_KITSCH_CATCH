#pragma once
#include "Include.h"
#include "gameIntro.h"

class Cartoon{
public:
	//! ******************** 생성자 소멸자 ******************** 
	Cartoon();
	~Cartoon();

	//! ************************** 기본 함수 **************************
	void FadeIn(double seconds);
	void CrossFade(ImgClass nextImgClass, double seconds);
	void Delay(double seconds);

	//! ************************** 변수 초기화 함수 **************************
	void InitFade();
	void InitCrossFade();
	void InitDelay();

	//! ************************** getter & setter **************************
	ImgClass GetImgClass() const { return img_class; }

	void SetImgClassSurface(SDL_Surface* new_surface) { img_class.SetSurface(new_surface); }
	void SetImgClassTexture(SDL_Texture* new_texture) { img_class.SetTexture(new_texture); }
	void SetImgClassSrcRect(SDL_Rect new_srcrect) { img_class.SetSrcRect(new_srcrect); }
	void SetImgClassDstRect(SDL_Rect new_dstrect) { img_class.SetDstRect(new_dstrect); }

	bool isFadeFinished() const { return is_fade_finished; }
	bool isCrossFadeFinished() const { return is_crossfade_finished; }
	bool isDelayFinished() const { return is_delay_finished; }

private:
	ImgClass img_class;

	// FadeIn() 변수
	double fade_delta;
	double fade_value;
	int fade_alpha;
	bool is_fade_finished;

	// CrossFade() 변수
	double crossfade_delta;
	double crossfade_value;
	int crossfade_alpha1;
	int crossfade_alpha2;
	bool is_crossfade_finished;

	// Delay() 변수
	double delay_elapsed_time;
	bool is_delay_finished;
};