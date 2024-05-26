#include "gamePlay.h"

extern SDL_Texture* exp_text; //"EXP" text


gamePlay::gamePlay() {

	isForcedQuit = false;
	isChanged = false;
	sec = 0;
	count_ = 0;
	stage = 1;

	

	
	//isChanged = false;
	isSetting = false;
	isVolumeOff = false;

	// BG
	SDL_Surface* bg_surface = IMG_Load("../../res/game page/game_background.png");
	play_bg = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);



	
	//cat
	SDL_Surface* cat_surface = IMG_Load("../../res/main page/main_cat1.png");
	cat = SDL_CreateTextureFromSurface(g_renderer, cat_surface);

	cat_rect.x = 0;
	cat_rect.y = 0;
	cat_rect.w = cat_surface->w;
	cat_rect.h = cat_surface->h;

	SDL_FreeSurface(cat_surface);



	SDL_Surface* cat_surface2 = IMG_Load("../../res/main page/main_cat2.png");
	cat2 = SDL_CreateTextureFromSurface(g_renderer, cat_surface2);

	cat_rect2.x = 0;
	cat_rect2.y = 0;
	cat_rect2.w = cat_surface2->w;
	cat_rect2.h = cat_surface2->h;

	SDL_FreeSurface(cat_surface2);

	
	{ //time bar
		timebar_rect.x = 0;
		timebar_rect.y = 290;
		timebar_rect.w = 540;
		timebar_rect.h = g_window_margin;
	}

	plus_score_int = org_score_int;

	{//setting img
		//mainscreen setting
		SDL_Surface* set_surface = IMG_Load("../../res/setting/setting_background.png");
		setting = SDL_CreateTextureFromSurface(g_renderer, set_surface);

		setting_rect.x = 0;
		setting_rect.y = 0;
		setting_rect.w = set_surface->w;
		setting_rect.h = set_surface->h;

		SDL_FreeSurface(set_surface);

	}

	//music
	play_music = Mix_LoadMUS("../../res/testRes/testBGM3.mp3");
	if (play_music == 0) {
		cout << "Mix_LoadMUS(\"testBGM2.mp3\"): " << Mix_GetError() << endl;
	}
	//Mix_VolumeMusic(128);
	//Mix_PlayMusic(play_music, -1);

	setting_SoundEffect = Mix_LoadWAV("../../res/testRes/testSound.mp3");

	//! ************************** gameLogic **************************
	m_gameLogic.InitGame(); //~ 생성자 호출시 게임로직 게임 초기화 함수 실행
	
}

gamePlay::~gamePlay() {
	Mix_FreeMusic(play_music);
	Mix_FreeChunk(setting_SoundEffect);
	//SDL_DestroyTexture(exp_text);
	SDL_DestroyTexture(score_text2);
	SDL_DestroyTexture(play_bg);
	SDL_DestroyTexture(cat);
	SDL_DestroyTexture(cat2);
	SDL_DestroyTexture(setting);
	//SDL_DestroyTexture(setting_bt);
	//SDL_DestroyTexture(timebar_bg);
	//SDL_DestroyTexture(stack_img);
	//SDL_DestroyTexture(playground_img);
	SDL_Quit();
	TTF_Quit();

	//! ************************** gameLogic **************************
	m_gameLogic.ClearGame(); //~ 소멸자 호출시 게임로직 게임 클리어 함수 실행
}

void gamePlay::HandleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_SPACE) {
				isChanged = true;
				isForcedQuit = true;
				SDL_Delay(33);

				changePhaseToEnding();
				
			}
			else {
				Mix_PlayMusic(play_music, -1);
			}break;

		default: break;
		}


		if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				int mouseX = event.button.x;
				int mouseY = event.button.y;


				//setting key
				if (isSetting == false && mouseX > setting_bt_rect.x && mouseY > setting_bt_rect.y &&
					mouseX < setting_bt_rect.x + setting_bt_rect.w && mouseY < setting_bt_rect.y + setting_bt_rect.h) {
					isSetting = !isSetting;
					Mix_PlayChannel(-1, setting_SoundEffect, 0);
				}

				if (isSetting == true) {
					if ( mouseX > set_Xkey_rect.x && mouseY > set_Xkey_rect.y &&
						mouseX < set_Xkey_rect.x + set_Xkey_rect.w && mouseY < set_Xkey_rect.y + set_Xkey_rect.h) {
						isSetting = !isSetting;
						Mix_PlayChannel(-1, setting_SoundEffect, 0);
					}

					if (mouseX > volume_bt_rect.x && mouseY > volume_bt_rect.y &&
						mouseX < volume_bt_rect.x + volume_bt_rect.w && mouseY < volume_bt_rect.y + volume_bt_rect.h) {
						isVolumeOff = !isVolumeOff;
						Mix_PlayChannel(-1, setting_SoundEffect, 0);
						if (isVolumeOff == true) {
							Mix_PauseMusic();
						}
						else if (isVolumeOff == false) {
							Mix_ResumeMusic();
						}
					}

					//resume
					if (mouseX > resume_rect.x && mouseY > resume_rect.y &&
						mouseX < resume_rect.x + resume_rect.w && mouseY < resume_rect.y + resume_rect.h) {
						isSetting = !isSetting;
						Mix_PlayChannel(-1, setting_SoundEffect, 0);
					}

					//go to home
					if (mouseX > home_rect.x && mouseY > home_rect.y &&
						mouseX < home_rect.x + home_rect.w && mouseY < home_rect.y + home_rect.h) {
						Mix_PlayChannel(-1, setting_SoundEffect, 0);
						isSetting = false;
						isForcedQuit = true;
						string score;
						plus_score_int = org_score_int;
						score = original_score;
						ofstream ofs;
						ofs.open("../../res/testRes/scoreboard.txt");
						ofs << score;
						ofs.close();
						isChanged = true;
						SDL_Delay(33);

						changePhaseToMain();
					}
				}
			}
			else if (isSetting == false && event.button.button == SDL_BUTTON_RIGHT) {
				plus_score_int += 10;
			}
		}
	}
	//! ************************** gameLogic **************************
	m_gameLogic.HandleEvents(); //~ 게임로직 이벤트 처리 함수 실행
}

void gamePlay::Update() {
	switch (stage) {
	default: limit_sec = 10; break;
	}
	string score;
	updateScore(plus_score_int);

	if (isForcedQuit) {
		isForcedQuit = false;
	}
	 
	
	
	
	if (isChanged == false) {
		
		if (isSetting) {}
		else {
			

			count_ += 1;
			if (count_ != 0 && count_ % 33 == 0) {
				sec += 1; //play second
				timebar_rect.w = timebar_rect.w - 540 / limit_sec;

				if (last_sec == 0) {
					SDL_Delay(1000);
					isChanged = true;
					//isForcedQuit = true;
					changePhaseToEnding();

				}
				
			}
		}
	}
	else if (isChanged) { 
		timebar_rect.w = 540; 

		
		

	}

	last_sec = limit_sec - sec;

	cout << "sec : " << sec << " last sec : " << last_sec << endl;

	//! ************************** gameLogic **************************
	m_gameLogic.Update(); //~ 게임로직 업데이트 함수 실행
}

void gamePlay::Render() {
	SDL_RenderCopy(g_renderer, play_bg, NULL, NULL);


	//time bar
	SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255); //red
	SDL_RenderFillRect(g_renderer, &timebar_rect);



	if (cat_status == false) {
		//cat image 1
		SDL_Rect tmp_r;
		tmp_r.x = 135;
		tmp_r.y = -110;
		tmp_r.w = cat_rect.w * 0.45;
		tmp_r.h = cat_rect.h * 0.45;
		SDL_RenderCopy(g_renderer, cat, &cat_rect, &tmp_r);

	}
	else {
		//cat image 2
		SDL_Rect tmp_r1;
		tmp_r1.x = 135;
		tmp_r1.y = -110;
		tmp_r1.w = cat_rect2.w * 0.45;
		tmp_r1.h = cat_rect2.h * 0.45;
		SDL_RenderCopy(g_renderer, cat2, &cat_rect2, &tmp_r1);
	}

	
	{ //score text
		SDL_Rect tmp_r;
		tmp_r.x = g_window_margin + 80;
		tmp_r.y = 37;
		tmp_r.w = score_rect.w * 0.8;
		tmp_r.h = score_rect.h * 0.8;
		SDL_RenderCopy(g_renderer, score_text2, &score_rect, &tmp_r);
	}

	if (isSetting == true) {
		SDL_Rect tmp_r;
		tmp_r.x = 0;
		tmp_r.y = 0;
		tmp_r.w = setting_rect.w;
		tmp_r.h = setting_rect.h;
		SDL_RenderCopy(g_renderer, setting, &setting_rect, &tmp_r);
		
		if (isVolumeOff == false) {
			SDL_RenderCopy(g_renderer, volume_bt_off, &volume_rect, &volume_rect);
		}
		else if (isVolumeOff == true) {
			SDL_RenderCopy(g_renderer, volume_bt_on, &volume_rect, &volume_rect);
		}
	}
	//! ************************** gameLogic **************************
	m_gameLogic.Render(); //~ 게임로직 렌더 함수 실행

	SDL_RenderPresent(g_renderer);
}

void gamePlay::changePhaseToEnding() {
	g_current_game_phase = PHASE_ENDING_CLEAR;
	clear_reset = true;
	Mix_HaltMusic();
	Mix_PlayMusic(clear_music, -1);

}

void gamePlay::changePhaseToMain() {
	g_current_game_phase = PHASE_MAIN;
	Mix_HaltMusic();
	Mix_PlayMusic(main_music, -1);
}

void gamePlay::play_timer(int interval) {
	//90s = 90000
	// 
	//const Uint32* pointer = &g_last_time_ms;
	
}

void gamePlay::updateScore(int s) {
	string front_score;
	update_score = to_string(s);

	//���� ���ڸ����� ����
	if (s == 0) {
		front_score = "000";
		update_score = front_score +  update_score;
	}
	else if (s > 0 && s < 10) {
		front_score = "000";
		update_score = front_score +  update_score;
	}
	else if (s >= 10 && s < 100) {
		front_score = "00";
		update_score = front_score +  update_score;
	}
	else if (s >= 100 && s < 1000) {
		front_score = "0";
		update_score = front_score +  update_score;
	}
	else { update_score = to_string(s); }


	TTF_Font* font = TTF_OpenFont("../../res/testRes/Galmuri14.ttf", 30);
	SDL_Color white = { 255,255,255,0 };
	SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(font, update_score.c_str(), white);
	//to_string(score).c_str()
	score_rect.x = 0;
	score_rect.y = 0;
	score_rect.w = tmp_surface->w;
	score_rect.h = tmp_surface->h;

	score_text2 = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);
	TTF_CloseFont(font);
}