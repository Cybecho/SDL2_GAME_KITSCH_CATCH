#pragma once

#include "Include.h"

class gameClass {
public:
	gameClass();
	virtual ~gameClass();

	virtual void HandleEvents() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	
	bool isSetting;
	bool isVolumeOff;

	int setting_bt_w = 310;
	int setting_bt_h = 70;

	
	SDL_Texture* volume_bt_off;
	SDL_Texture* volume_bt_on;
	SDL_Rect setting_rect;
	SDL_Rect setting_bt_rect;
	SDL_Rect set_Xkey_rect;
	SDL_Rect volume_bt_rect; //click area
	SDL_Rect volume_rect; //image source rect
	SDL_Rect resume_rect;
	SDL_Rect home_rect;
	
};

class Timer {
public:
	Timer();
	~Timer();

	void setInterval(int interval); //주기 설정
	void start();
	void pause();
	void resume();
	bool done(); //시간 다 되었는지 조사

private:
	double interval;
	double startTicks;
	double pauseTicks;
};

// ImgRect: 이미지 생성에 필요한 변수들을 class로 묶음
class ImgClass {
public:
	//! ******************** 생성자 소멸자 ******************** 
	ImgClass();
	~ImgClass();

	//! ************************** getter & setter **************************
	SDL_Surface* GetSurface() const { return surface; }
	SDL_Texture* GetTexture() const { return texture; }
	SDL_Rect GetSrcRect() const { return srcrect; }
	SDL_Rect GetDstRect() const { return dstrect; }

	void SetSurface(SDL_Surface* new_surface) { surface = new_surface; }
	void SetTexture(SDL_Texture* new_texture) { texture = new_texture; }
	void SetSrcRect(SDL_Rect new_srcrect) { srcrect = new_srcrect; }
	void SetDstRect(SDL_Rect new_dstrect) { dstrect = new_dstrect; }

private:
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect srcrect;
	SDL_Rect dstrect;
};

void InitGame();
void ClearGame();

extern int g_current_game_phase;
extern bool g_flag_running;
extern Uint32 g_last_time_ms;
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;





