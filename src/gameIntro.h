#pragma once

#include "Include.h"
#include "gameClass.h"
#include "obj_gameIntroCartoon.h"

// Number of cartoons
#define cartoon_cut 3

class Cartoon; // Prevent compile error
class gameIntro : public gameClass {
public:
	gameIntro();
	~gameIntro();

	void HandleEvents();
	void Update();
	void Render();

	std::vector<Cartoon> cartoon;
};

extern Mix_Music* intro_music;
extern Mix_Music* clear_music;