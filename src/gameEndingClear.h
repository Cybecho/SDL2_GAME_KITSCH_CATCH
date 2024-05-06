#pragma once

#include "SDL.h"
#include "gameClass.h"

class gameEndingClear : public gameClass {
public:
	gameEndingClear();
	~gameEndingClear();

	void HandleEvents();
	void Update();
	void Render();
};

extern Mix_Music* intro_music;
extern Mix_Music* clear_music;
extern Mix_Music* gameover_music;