#pragma once

#include "include.h"
#include "gameClass.h"

class gameEndingClear : public gameClass {
public:
	gameEndingClear();
	~gameEndingClear();

	void HandleEvents();
	void Update();
	void Render();
};
extern Mix_Music* clear_music;