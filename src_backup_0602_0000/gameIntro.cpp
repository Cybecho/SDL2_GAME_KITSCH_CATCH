#include "gameIntro.h"

bool intro_reset = false;

gameIntro::gameIntro() {
	cartoon.resize(CARTOON_IMG);

	// 만화 이미지 로드
	cartoon[0].SetImgClassSurface(IMG_Load("../../res/intro_cartoon/intro.png"));
	cartoon[1].SetImgClassSurface(IMG_Load("../../res/intro_cartoon/intro_cartoon1.png"));
	cartoon[2].SetImgClassSurface(IMG_Load("../../res/intro_cartoon/intro_cartoon2.png"));
	cartoon[3].SetImgClassSurface(IMG_Load("../../res/intro_cartoon/intro_cartoon3.png"));
	cartoon[4].SetImgClassSurface(IMG_Load("../../res/intro_cartoon/intro_cartoon4.png"));
	cartoon[5].SetImgClassSurface(IMG_Load("../../res/intro_cartoon/intro_cartoon5.png"));
	cartoon[6].SetImgClassSurface(IMG_Load("../../res/intro_cartoon/intro_cartoon6.png"));

	// 텍스처 생성
	for (auto& i : cartoon) {
		i.SetImgClassTexture(SDL_CreateTextureFromSurface(g_renderer, i.GetImgClass().surface));
		i.SetImgClassSrcRect({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT });
		i.SetImgClassDstRect({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT });
		SDL_FreeSurface(i.GetImgClass().surface);
	}

	// BGM and SoundEffect
	intro_music = Mix_LoadMUS("../../res/testRes/testBGM1.mp3");
	if (intro_music == 0) {
		printf("Couldn't load the wav: %s\n", Mix_GetError());
	}
	//Mix_VolumeMusic(128);
	Mix_PlayMusic(intro_music, -1);
}

gameIntro::~gameIntro() {
	Mix_FreeMusic(intro_music);
	for (auto& i : cartoon) {
		SDL_DestroyTexture(i.GetImgClass().texture);
	}
}

void gameIntro::HandleEvents() {
	// Debug Print
	//cout << g_current_game_phase << endl;

	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			// Spacebar (cheat key)
			if (event.key.keysym.sym == SDLK_SPACE) {
				cout << "Cheat key has been used!" << endl;
				g_current_game_phase = PHASE_MAIN;
				Mix_HaltMusic();
				Mix_PlayMusic(main_music, -1);
			}
			// Any keys (except spacebar)
			else {
				if (is_cartoon_finished) {
					g_current_game_phase = PHASE_MAIN;
					Mix_HaltMusic();
					Mix_PlayMusic(main_music, -1);
				}
			}
			break;

			

		default:
			break;
		}

		if (event.type == SDL_MOUSEBUTTONDOWN) {

			if (is_cartoon_finished && event.button.button == SDL_BUTTON_LEFT) {
				g_current_game_phase = PHASE_MAIN;
				Mix_HaltMusic();
				Mix_PlayMusic(main_music, -1);
			}
		}

	}
}

void gameIntro::Update() {
	if (intro_reset) {
		for (int i = 0; i < CARTOON_IMG-1; i++) {
			cartoon[i].InitFade();
			cartoon[i].InitCrossFade();
			cartoon[i].InitDelay();
		}
		intro_reset = false;
	}
}

void gameIntro::Render() {
	// BG
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	SDL_RenderClear(g_renderer);

	cartoon[0].FadeIn(0.5);
	if (cartoon[0].isFadeFinished())
		cartoon[0].Delay(1);

	if (cartoon[0].isDelayFinished())
		cartoon[1].FadeIn(0.3);
	if (cartoon[1].isFadeFinished())
		cartoon[1].Delay(3);

	if (cartoon[1].isDelayFinished())
		cartoon[2].FadeIn(0.5);
	if (cartoon[2].isFadeFinished())
		cartoon[2].Delay(1);

	if (cartoon[2].isDelayFinished())
		cartoon[3].FadeIn(0.5);
	if (cartoon[3].isFadeFinished())
		cartoon[3].Delay(2);

	if (cartoon[3].isDelayFinished())
		cartoon[4].FadeIn(0.5);
	if (cartoon[4].isFadeFinished())
		cartoon[4].Delay(5);

	if (cartoon[4].isDelayFinished())
		cartoon[5].FadeIn(0.5);
	if (cartoon[5].isFadeFinished())
		cartoon[5].Delay(2);

	if (cartoon[5].isDelayFinished())
		cartoon[6].FadeIn(0.5);

	// Prevent skipping cartoon
	if (cartoon[6].isFadeFinished())
		is_cartoon_finished = true;

	// Draw to the screen
	SDL_RenderPresent(g_renderer);
}