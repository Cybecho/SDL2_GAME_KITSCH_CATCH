#include "obj_gameIntroCartoon.h"

// Cartoon 클래스
// 해당 이미지에 부드러운 트랜지션 효과를 적용합니다.

Cartoon::Cartoon() {
	// Init FadeIn() Variables
	fade_alpha = fade_value = fade_delta = is_fade_finished = 0;

	// Init CrossFade() Variables
	crossfade_alpha1 = crossfade_delta = is_crossfade_finished = 0;
	crossfade_alpha2 = 255;
	crossfade_value = 1;

	// Init Delay() Variables
	delay_elapsed_time = is_delay_finished = 0;
}

Cartoon::~Cartoon() {
}

ImgClass Cartoon::GetImgClass() {
	return img_class;
}

void Cartoon::SetImgClassSurface(SDL_Surface* new_surface) {
	img_class.surface = new_surface;
}

void Cartoon::SetImgClassTexture(SDL_Texture* new_texture) {
	img_class.texture = new_texture;
}

void Cartoon::SetImgClassSrcRect(SDL_Rect new_srcrect) {
	img_class.srcrect = new_srcrect;
}

void Cartoon::SetImgClassDstRect(SDL_Rect new_dstrect) {
	img_class.dstrect = new_dstrect;
}

// 해당 이미지를 seconds동안 화면 상단에 FadeIn 트랜지션을 적용합니다.
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
	SDL_SetTextureBlendMode(img_class.texture, SDL_BLENDMODE_BLEND); // Required to apply ALPHA VALUE
	SDL_SetTextureAlphaMod(img_class.texture, fade_alpha);
	SDL_RenderCopy(g_renderer, img_class.texture, &img_class.srcrect, &img_class.dstrect);
}

/** CrossFade() 업데이트
*	두 이미지를 서로 CrossFade했을 때 어두어졌다가 밝아지는 문제가 있었습니다.
*	이는 이미지가 투명해지면서 배경이 보여지는게 원인으로 추정됩니다.
*	해결방안1: 시그모이드 함수를 사용하여 alpha값이 순간적으로 빠르게 변화하도록 변경했습니다.
*	해결방안2: SDL_RenderCopy를 두 겹씩 적용했습니다.
**/

// 해당 이미지(A)를 next_img_class 이미지(B)로 seconds동안 CrossFade 트랜지션을 적용합니다.
void Cartoon::CrossFade(ImgClass next_img_class, double seconds) {

	crossfade_delta = 1 / (seconds * 33.);
	if (crossfade_value - crossfade_delta >= 0) { // crossfade_value: 1 -> 0으로 작아짐
		crossfade_value = crossfade_value - crossfade_delta;
		crossfade_alpha1 = (int)((1 / (1 + std::exp(-10 * (crossfade_value - 0.5)))) * 255);
		crossfade_alpha2 = (int)((1 / (1 + std::exp(+10 * (crossfade_value - 0.5)))) * 255);
	}
	else {
		crossfade_alpha1 = 0;
		crossfade_alpha2 = 255;
		is_crossfade_finished = true;
	}

	SDL_SetTextureBlendMode(img_class.texture, SDL_BLENDMODE_BLEND); // Required to apply ALPHA VALUE
	SDL_SetTextureBlendMode(next_img_class.texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(img_class.texture, crossfade_alpha1);
	SDL_SetTextureAlphaMod(next_img_class.texture, crossfade_alpha2);
	SDL_RenderCopy(g_renderer, img_class.texture, &img_class.srcrect, &img_class.dstrect);
	SDL_RenderCopy(g_renderer, img_class.texture, &img_class.srcrect, &img_class.dstrect);
	SDL_RenderCopy(g_renderer, next_img_class.texture, &next_img_class.srcrect, &next_img_class.dstrect);
	SDL_RenderCopy(g_renderer, next_img_class.texture, &next_img_class.srcrect, &next_img_class.dstrect);
}

// 해당 이미지를 seconds동안 지연시키는 타이머를 적용합니다.
// SDL 게임 특성상 sleep()을 사용하면 게임이 먹통이 되므로 이 기능을 사용합니다.
// 타이머가 끝나면 is_delay_finished가 true로 바뀝니다.
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
	crossfade_alpha1 = crossfade_delta = is_crossfade_finished = 0;
	crossfade_alpha2 = 255;
	crossfade_value = 1;
}

void Cartoon::InitDelay() {
	delay_elapsed_time = is_delay_finished = 0;
}