#include "gameEndingClear.h"

bool clear_reset = false;

gameEndingClear::gameEndingClear() {
	bg.resize(CLEAR_BG_IMG);
	cat.resize(CLEAR_CAT_IMG);
	mainBT_rect = { 73,424,173,55 };
	retryBT_rect = { 293,424,173,55 };
	cat_cut = 0;
	bt_clickable = false;

	// 클리어 엔딩 이미지 로드
	bg[0].imgClass.surface = IMG_Load("../../res/ending/ending_clear/ending_clear_cut1.png");
	bg[1].imgClass.surface = IMG_Load("../../res/ending/ending_clear/ending_clear_cut2.png");
	cat[0].imgClass.surface = IMG_Load("../../res/ending/ending_clear/ending_clear_cat_1.png");
	cat[1].imgClass.surface = IMG_Load("../../res/ending/ending_clear/ending_clear_cat_2.png");
	cat[2].imgClass.surface = IMG_Load("../../res/ending/ending_clear/ending_clear_cat_3.png");
	cat[3].imgClass.surface = IMG_Load("../../res/ending/ending_clear/ending_clear_cat_4.png");
	cat[4].imgClass.surface = IMG_Load("../../res/ending/ending_clear/ending_clear_cat_5.png");
	cat[5].imgClass.surface = IMG_Load("../../res/ending/ending_clear/ending_clear_cat_6.png");

	// 텍스처 생성
	for (auto& i : bg) {
		i.imgClass.texture = SDL_CreateTextureFromSurface(g_renderer, i.imgClass.surface);
		i.imgClass.srcrect = i.imgClass.dstrect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
		SDL_FreeSurface(i.imgClass.surface);
	}
	for (auto& i : cat) {
		i.imgClass.texture = SDL_CreateTextureFromSurface(g_renderer, i.imgClass.surface);
		i.imgClass.srcrect = i.imgClass.dstrect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
		SDL_FreeSurface(i.imgClass.surface);
	}

	// BGM and SoundEffect
	clear_music = Mix_LoadMUS("../../res/testRes/testBGM2.mp3");
	if (clear_music == 0) {
		printf("Couldn't load the wav: %s\n", Mix_GetError());
	}
	SoundEffect = Mix_LoadWAV("../../res/testRes/testSound.mp3");
}

gameEndingClear::~gameEndingClear() {
	for (auto& i : bg) {
		SDL_DestroyTexture(i.imgClass.texture);
	}
	for (auto& i : cat) {
		SDL_DestroyTexture(i.imgClass.texture);
	}
	Mix_FreeMusic(clear_music);
	Mix_FreeChunk(SoundEffect);
}

void gameEndingClear::HandleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			// Spacebar (cheat key)
			if (event.key.keysym.sym == SDLK_SPACE) {
				std::cout << "Cheat key has been used!" << std::endl;
				g_current_game_phase = PHASE_ENDING_GAMEOVER;
				gameover_reset = true;
			}
			break;

		default:
			break;
		}

		// 버튼 작동 부분: 가을님 코드 활용
		if (event.type == SDL_MOUSEBUTTONDOWN && bt_clickable) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				int mouseX = event.button.x;
				int mouseY = event.button.y;

				//click main button
				if (mouseX > mainBT_rect.x && mouseY > mainBT_rect.y &&
					mouseX < mainBT_rect.x + mainBT_rect.w && mouseY < mainBT_rect.y + mainBT_rect.h) {
					g_current_game_phase = PHASE_MAIN;
					bt_clickable = false;
					Mix_PlayChannel(-1, SoundEffect, 0);
				}

				//click retry button
				if (mouseX > retryBT_rect.x && mouseY > retryBT_rect.y &&
					mouseX < retryBT_rect.x + retryBT_rect.w && mouseY < retryBT_rect.y + retryBT_rect.h) {
					g_current_game_phase = PHASE_PLAYING;
					bt_clickable = false;
					Mix_PlayChannel(-1, SoundEffect, 0);
				}
			}
		}
	}
}

void gameEndingClear::Update() {
	if (clear_reset) {
		for (int i = 0; i < CLEAR_BG_IMG - 1; i++) {
			bg[i].InitFade();
			bg[i].InitCrossFade();
			bg[i].InitDelay();
		}
		for (int i = 0; i < CLEAR_CAT_IMG - 1; i++) {
			cat[i].InitFade();
			cat[i].InitCrossFade();
			cat[i].InitDelay();
		}
		cat_cut = 0;
		bt_clickable = false;
		clear_reset = false;
	}
}

void gameEndingClear::Render() {
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	SDL_RenderClear(g_renderer); // clear the renderer to the draw color

	// 엔딩 만화 출력
	bg[0].FadeIn(1); 
	if (bg[0].is_fade_finished)
		bg[0].Delay(1.5);

	// 엔딩 화면 출력
	if(bg[0].is_delay_finished)
		bg[0].CrossFade(bg[1].imgClass, 0.5); // 0.5초간 다음 엔딩 그림과 CrossFade

	if (bg[0].is_crossfade_finished) {
		bt_clickable = true;

		// CrossFade()로 투명 고양이 무한 반복 출력
		if (cat_cut == CLEAR_CAT_IMG - 1) // 투명 고양이 그림이 마지막 그림이라면
			cat[cat_cut].CrossFade(cat[0].imgClass, 0.2); // 0.2초동안 첫번째 그림과 CrossFade
		else
			cat[cat_cut].CrossFade(cat[cat_cut + 1].imgClass, 0.2); // 0.2초동안 다음 그림과 CrossFade

		if(cat[cat_cut].is_crossfade_finished)
			cat[cat_cut].Delay(0.7);

		// 그림이 끝날때마다 다음 CrossFade를 위해 관련 함수 초기화
		if (cat[cat_cut].is_delay_finished) {
			cat[cat_cut].InitCrossFade();
			cat[cat_cut].InitDelay();
			
			cat_cut++;
			if(cat_cut == CLEAR_CAT_IMG) // 마지막 그림에 도달하면 첫번째 그림으로 초기화
				cat_cut = 0;
		}
	}

	SDL_RenderPresent(g_renderer); // draw to the screen
}