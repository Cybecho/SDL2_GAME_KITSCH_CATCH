#pragma once

#include "include.h"
#include "gameClass.h"
#include "gameIntroCartoon.h"

// 인트로 만화 컷 수
#define cartoon_cut 3 // 배열의 크기는 변수가 아닌 상수로 들어가야 함, 근데 const가 아닌 define으로 해야함

class Cartoon; // 컴파일 오류 발생 방지
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