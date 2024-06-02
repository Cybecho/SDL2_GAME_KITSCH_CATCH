#pragma once

#include "Include.h"
#include "gameClass.h"
#include "obj_gameIntroCartoon.h"
#define CARTOON_IMG 7

class Cartoon; // Prevent compile error
class gameIntro : public gameClass {
public:
	gameIntro();
	~gameIntro();

	void HandleEvents();
	void Update();
	void Render();

	std::vector<Cartoon> cartoon;
	bool is_cartoon_finished = false;
};
extern Mix_Music* intro_music;
extern Mix_Music* main_music;
extern Mix_Music* clear_music;