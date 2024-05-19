#include "gamePlay.h"

extern SDL_Texture* exp_text; //"EXP" text
extern SDL_Texture* score_text; //get score from txt file

gamePlay::gamePlay() {


	

	

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
		timebar_rect.w = 0;
		timebar_rect.h = g_window_margin;
	}

	{ //score text
		ifstream file("../../res/testRes/scoreboard.txt"); //read scoreboard
		getline(file, score);
		score_int = stoi(score);

		string front_score;
		string new_score;


		//점수 네자리수로 고정(나중에 구현)
		if (score_int == 0) {
			front_score = "000";
			new_score = front_score + score;
		}
		else if (score_int > 0 && score_int < 100) {
			front_score = "00";
			new_score = front_score + score;
		}
		else if (score_int >= 100 && score_int < 1000) {
			front_score = "0";
			new_score = front_score + score;
		}
		else { new_score = score; }

		TTF_Font* font = TTF_OpenFont("../../res/testRes/Galmuri14.ttf", 30);
		SDL_Color white = { 255,255,255,0 };
		SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(font, new_score.c_str(), white);
		//std::to_string(score).c_str()
		score_rect.x = 0;
		score_rect.y = 0;
		score_rect.w = tmp_surface->w;
		score_rect.h = tmp_surface->h;

		score_text = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
		SDL_FreeSurface(tmp_surface);
		TTF_CloseFont(font);
		file.close();
	}



	//music
	play_music = Mix_LoadMUS("../../res/testRes/testBGM3.mp3");
	if (play_music == 0) {
		std::cout << "Mix_LoadMUS(\"testBGM2.mp3\"): " << Mix_GetError() << std::endl;
	}
	//Mix_VolumeMusic(128);
	//Mix_PlayMusic(play_music, -1);

	setting_SoundEffect = Mix_LoadWAV("../../res/testRes/testSound.mp3");
}

gamePlay::~gamePlay() {
	Mix_FreeMusic(play_music);
	Mix_FreeChunk(setting_SoundEffect);
	//SDL_DestroyTexture(exp_text);
	SDL_DestroyTexture(score_text);
	SDL_DestroyTexture(play_bg);
	SDL_DestroyTexture(cat);
	SDL_DestroyTexture(cat2);
	//SDL_DestroyTexture(setting_bt);
	//SDL_DestroyTexture(timebar_bg);
	//SDL_DestroyTexture(stack_img);
	//SDL_DestroyTexture(playground_img);
	SDL_Quit();
	TTF_Quit();
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
						isSetting = false;
						Mix_PlayChannel(-1, setting_SoundEffect, 0);
						changePhaseToMain();
					}
				}
			}
		}
	}
}

void gamePlay::Update() {
	timebar_rect.w = timebarw;
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
		SDL_RenderCopy(g_renderer, score_text, &score_rect, &tmp_r);
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