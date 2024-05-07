#pragma once

#include "include.h"
#include "gameClass.h"

class gameEndingGameover : public gameClass {
public:
	gameEndingGameover();
	~gameEndingGameover();

	void HandleEvents();
	void Update();
	void Render();
};

extern Mix_Music* intro_music;
extern Mix_Music* gameover_music;