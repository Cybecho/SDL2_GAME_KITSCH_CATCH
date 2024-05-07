#pragma once

#include "include.h"
#include "gameClass.h"
#include "gameIntroCartoon.h"

// ��Ʈ�� ��ȭ �� ��
#define cartoon_cut 3 // �迭�� ũ��� ������ �ƴ� ����� ���� ��, �ٵ� const�� �ƴ� define���� �ؾ���

class Cartoon; // ������ ���� �߻� ����
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