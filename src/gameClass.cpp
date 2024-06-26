#include "gameClass.h"

gameClass::gameClass() {
	


	{ //setting button area
		setting_bt_rect.x = 460;
		setting_bt_rect.y = 20;
		setting_bt_rect.w = 60;
		setting_bt_rect.h = 60;
	}

	{//setting X key area
		set_Xkey_rect.x = 410;
		set_Xkey_rect.y = 240;
		set_Xkey_rect.w = 30;
		set_Xkey_rect.h = 30;
	}

	{//setting volume button click area
		volume_bt_rect.x = 115;
		volume_bt_rect.y = 360;
		volume_bt_rect.w = setting_bt_w;
		volume_bt_rect.h = setting_bt_h;
	}

	{//resume button click area
		resume_rect.x = 115;
		resume_rect.y = 460;
		resume_rect.w = setting_bt_w;
		resume_rect.h = setting_bt_h;
	}

	{//home button click area
		home_rect.x = 115;
		home_rect.y = 560;
		home_rect.w = setting_bt_w;
		home_rect.h = setting_bt_h;
	}


	{//난이도 조절 창 : 첫 번째 버튼 영역
		difficulty_rect.x = 90;
		difficulty_rect.y = 270;
		difficulty_rect.w = 360;
		difficulty_rect.h = 80;
	}

	{//난이도 조절하는 단계 이미지
		SDL_Surface* surface = IMG_Load("../../res/game_mode/game_mode_fin.png");
		difficulty_bg = SDL_CreateTextureFromSurface(g_renderer, surface);
		SDL_FreeSurface(surface);
	}
	
	

	{ //bt on
		SDL_Surface* volume_on_surface = IMG_Load("../../res/setting/setting_btn_on.png");
		volume_bt_on = SDL_CreateTextureFromSurface(g_renderer, volume_on_surface);

		volume_rect.x = 0;
		volume_rect.y = 0;
		volume_rect.w = volume_on_surface->w;
		volume_rect.h = volume_on_surface->h;

		SDL_FreeSurface(volume_on_surface);
	}

	{ //bt off
		SDL_Surface* volume_off_surface = IMG_Load("../../res/setting/setting_btn_off.png");
		volume_bt_off = SDL_CreateTextureFromSurface(g_renderer, volume_off_surface);

		volume_rect.x = 0;
		volume_rect.y = 0;
		volume_rect.w = volume_off_surface->w;
		volume_rect.h = volume_off_surface->h;

		SDL_FreeSurface(volume_off_surface);
	}

	cat_sound = Mix_LoadWAV("../../res/music/cat_play_sound1.mp3");
	// 효과음 볼륨 설정 (0 ~ 128)
	Mix_VolumeChunk(cat_sound, 50); 
	cat_sound2 = Mix_LoadWAV("../../res/music/cat_play_sound2.mp3");
	Mix_VolumeChunk(cat_sound2, 50); 
}

gameClass::~gameClass() {
	SDL_DestroyTexture(difficulty_bg);
	SDL_DestroyTexture(volume_bt_off);
	SDL_DestroyTexture(volume_bt_on);
	Mix_FreeChunk(cat_sound);
	Mix_FreeChunk(cat_sound2);
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

ImgClass::ImgClass() {
	surface = 0;
	texture = 0;
	srcrect = { 0, };
	dstrect = { 0, };
}

ImgClass::~ImgClass() {
}