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
	bg[0].SetImgClassSurface(IMG_Load("../../res/ending/ending_clear/ending_clear_cut1.png"));
	bg[1].SetImgClassSurface(IMG_Load("../../res/ending/ending_clear/ending_clear_cut2.png"));
	cat[0].SetImgClassSurface(IMG_Load("../../res/ending/ending_clear/ending_clear_cat_1.png"));
	cat[1].SetImgClassSurface(IMG_Load("../../res/ending/ending_clear/ending_clear_cat_2.png"));
	cat[2].SetImgClassSurface(IMG_Load("../../res/ending/ending_clear/ending_clear_cat_3.png"));
	cat[3].SetImgClassSurface(IMG_Load("../../res/ending/ending_clear/ending_clear_cat_4.png"));
	cat[4].SetImgClassSurface(IMG_Load("../../res/ending/ending_clear/ending_clear_cat_5.png"));
	cat[5].SetImgClassSurface(IMG_Load("../../res/ending/ending_clear/ending_clear_cat_6.png"));

	// 텍스처 생성
	for (auto& i : bg) {
		i.SetImgClassTexture(SDL_CreateTextureFromSurface(g_renderer, i.GetImgClass().GetSurface()));
		i.SetImgClassSrcRect({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT });
		i.SetImgClassDstRect({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT });
		SDL_FreeSurface(i.GetImgClass().GetSurface());
	}
	for (auto& i : cat) {
		i.SetImgClassTexture(SDL_CreateTextureFromSurface(g_renderer, i.GetImgClass().GetSurface()));
		i.SetImgClassSrcRect({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT });
		i.SetImgClassDstRect({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT });
		SDL_FreeSurface(i.GetImgClass().GetSurface());
	}

	// BGM and SoundEffect
	clear_music = Mix_LoadMUS("../../res/music/clear_music.mp3");
	if (clear_music == 0) {
		printf("Couldn't load the wav: %s\n", Mix_GetError());
	}
	SoundEffect = Mix_LoadWAV("../../res/testRes/testSound.mp3");
	// 효과음 볼륨 설정 (0 ~ 128)
	Mix_VolumeChunk(SoundEffect, 12);  // 배경음 볼륨 10% 수준으로 설정
}

gameEndingClear::~gameEndingClear() {
	for (auto& i : bg) {
		SDL_DestroyTexture(i.GetImgClass().GetTexture());
	}
	for (auto& i : cat) {
		SDL_DestroyTexture(i.GetImgClass().GetTexture());
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
				cout << "Cheat key has been used!" << endl;
				g_current_game_phase = PHASE_ENDING_GAMEOVER;
				gameover_reset = true;
				Mix_HaltMusic();
				Mix_PlayMusic(gameover_music, -1);
			}
			break;

		default:
			break;
		}

		// 버튼 동작 부분: 아래의 코드 활성
		if (event.type == SDL_MOUSEBUTTONDOWN && bt_clickable) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				int mouseX = event.button.x;
				int mouseY = event.button.y;

				//click main button
				if (mouseX > mainBT_rect.x && mouseY > mainBT_rect.y &&
					mouseX < mainBT_rect.x + mainBT_rect.w && mouseY < mainBT_rect.y + mainBT_rect.h) {
					g_current_game_phase = PHASE_MAIN;
					clear_reset = true; // 추가
					bt_clickable = false;
					Mix_PlayChannel(-1, SoundEffect, 0);
					Mix_HaltMusic();
					Mix_PlayMusic(main_music, -1);
				}

				//click retry button
				if (mouseX > retryBT_rect.x && mouseY > retryBT_rect.y &&
					mouseX < retryBT_rect.x + retryBT_rect.w && mouseY < retryBT_rect.y + retryBT_rect.h) {
					g_current_game_phase = PHASE_PLAYING;
					clear_reset = true; // 추가
					bt_clickable = false;
					Mix_PlayChannel(-1, SoundEffect, 0);
					Mix_HaltMusic();
					Mix_PlayMusic(play_music, -1);
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

	isChanged = false;
	timebar_rect.w = 540;
	sec = 0;
}

void gameEndingClear::Render() {
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	SDL_RenderClear(g_renderer); // clear the renderer to the draw color

	if (!clear_reset) { // 1틱동안 이전 엔딩 화면이 뜨는 오류 방지

		// 배경 등장 부분
		bg[0].FadeIn(1);
		if (bg[0].isFadeFinished()) {
			bg[0].Delay(1.5);

			// 배경 전환 부분
			if (bg[0].isDelayFinished()) {
				bg[0].CrossFade(bg[1].GetImgClass(), 0.5); // 0.5초간 다음 배경 이미지 CrossFade
			}

			if (bg[0].isCrossFadeFinished()) {
				bt_clickable = true;

				// CLEAR_BG_IMG 애니메이션이 완료된 후에 CLEAR_CAT_IMG 애니메이션 시작
				if (cat_cut == CLEAR_CAT_IMG - 1)
					cat[cat_cut].CrossFade(cat[0].GetImgClass(), 0.1);
				else
					cat[cat_cut].CrossFade(cat[cat_cut + 1].GetImgClass(), 0.1);

				if (cat[cat_cut].isCrossFadeFinished())
					cat[cat_cut].Delay(0.7);

				if (cat[cat_cut].isDelayFinished()) {
					cat[cat_cut].InitCrossFade();
					cat[cat_cut].InitDelay();

					cat_cut++;
					if (cat_cut == CLEAR_CAT_IMG)
						cat_cut = 0;
				}
			}
		}
	}

	SDL_RenderPresent(g_renderer); // draw to the screen
}