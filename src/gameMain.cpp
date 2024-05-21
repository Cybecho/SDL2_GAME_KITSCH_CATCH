#include "gameMain.h"

SDL_Texture* exp_text; //"EXP" text
SDL_Texture* score_text; //get score from txt file




gameMain::gameMain() {
	isRule = false;
	isSetting = false;
	isVolumeOff = false;

	// BG
	SDL_Surface* bg_surface = IMG_Load("../../res/main page/main_background_2.png");
	main_bg = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);


	//playbutton area
	{
		playBT_dir_rect.x = 140;
		playBT_dir_rect.y = 850;
		playBT_dir_rect.w = 260;
		playBT_dir_rect.h = 90;
	}


	//question mark area
	{
		ques_rect.x = 10;
		ques_rect.y = 870;
		ques_rect.w = 50;
		ques_rect.h = 50;
	}

	{//rule X key area
		rule_Xkey_rect.x = 430;
		rule_Xkey_rect.y = 120;
		rule_Xkey_rect.w = 30;
		rule_Xkey_rect.h = 30;
	}

	

	//rule image
	SDL_Surface* rule_surface = IMG_Load("../../res/main page/rule.png");
	rule = SDL_CreateTextureFromSurface(g_renderer, rule_surface);

	rule_rect.x = 0;
	rule_rect.y = 0;
	rule_rect.w = rule_surface->w;
	rule_rect.h = rule_surface->h;

	SDL_FreeSurface(rule_surface);

	

	//cat
	SDL_Surface* cat_surface = IMG_Load("../../res/main page/main_sprite.png");
	cat = SDL_CreateTextureFromSurface(g_renderer, cat_surface);

	cat_rect.x = 0;
	cat_rect.y = 0;
	cat_rect.w = cat_surface->w;
	cat_rect.h = cat_surface->h;

	SDL_FreeSurface(cat_surface);


	

	{ //score text
		ifstream file("../../res/testRes/scoreboard.txt"); //read scoreboard
		getline(file, score);
		int score_int = stoi(score);

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



	{//setting img
		//mainscreen setting
			SDL_Surface* set_surface = IMG_Load("../../res/setting/main_setting_background.png");
			setting = SDL_CreateTextureFromSurface(g_renderer, set_surface);

			setting_rect.x = 0;
			setting_rect.y = 0;
			setting_rect.w = set_surface->w;
			setting_rect.h = set_surface->h;

			SDL_FreeSurface(set_surface);
		
	}



	//music
	main_music = Mix_LoadMUS("../../res/testRes/testBGM2.mp3");
	if (main_music == 0) {
		std::cout << "Mix_LoadMUS(\"testBGM2.mp3\"): " << Mix_GetError() << std::endl;
	}
	//Mix_VolumeMusic(128);
	


	//sound effect(play button)
	SoundEffect = Mix_LoadWAV("../../res/testRes/testSound.mp3");
}

gameMain::~gameMain() {
	Mix_FreeMusic(main_music);
	Mix_FreeChunk(SoundEffect);
	//SDL_DestroyTexture(exp_text);
	SDL_DestroyTexture(score_text);
	SDL_DestroyTexture(main_bg);
	SDL_DestroyTexture(cat);
	SDL_DestroyTexture(setting);
	//SDL_DestroyTexture(inter_bt);
	//SDL_DestroyTexture(play_bt);
	//SDL_DestroyTexture(setting_bt);
	
	SDL_Quit();
	TTF_Quit();
}

void gameMain::HandleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {

		switch (event.type) {

		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_SPACE) {
				changePhaseToPlay();
			}
			else {
				Mix_PlayMusic(main_music, -1);
			}break;

		default: break;
		}

		if (event.type == SDL_MOUSEBUTTONDOWN) { 
			if (event.button.button == SDL_BUTTON_LEFT) {
				int mouseX = event.button.x;
				int mouseY = event.button.y;

				
				if (isRule == false && isSetting == false) { //main screen

					//click play button
					if (mouseX > playBT_dir_rect.x && mouseY > playBT_dir_rect.y &&
						mouseX < playBT_dir_rect.x + playBT_dir_rect.w && mouseY < playBT_dir_rect.y + playBT_dir_rect.h) {
						changePhaseToPlay();
						Mix_PlayChannel(-1, SoundEffect, 0);
					}

					//click question mark -> open rule page
					if (mouseX > ques_rect.x && mouseY > ques_rect.y &&
						mouseX < ques_rect.x + ques_rect.w && mouseY < ques_rect.y + ques_rect.h) {
						isRule = !isRule;
						Mix_PlayChannel(-1, SoundEffect, 0);

					}

					//setting key
					if ( mouseX > setting_bt_rect.x && mouseY > setting_bt_rect.y &&
						mouseX < setting_bt_rect.x + setting_bt_rect.w && mouseY < setting_bt_rect.y + setting_bt_rect.h) {
						isSetting = !isSetting;
						Mix_PlayChannel(-1, SoundEffect, 0);
					}
				}
				

					//click X -> back to main screen
					if (isRule == true && mouseX > rule_Xkey_rect.x && mouseY > rule_Xkey_rect.y &&
						mouseX < rule_Xkey_rect.x + rule_Xkey_rect.w && mouseY < rule_Xkey_rect.y + rule_Xkey_rect.h) {
						isRule = !isRule;
						Mix_PlayChannel(-1, SoundEffect, 0);
					}

					if (isSetting == true) {
						//click X key
						if (mouseX > set_Xkey_rect.x && mouseY > set_Xkey_rect.y &&
							mouseX < set_Xkey_rect.x + set_Xkey_rect.w && mouseY < set_Xkey_rect.y + set_Xkey_rect.h) {
							isSetting = !isSetting;
							Mix_PlayChannel(-1, SoundEffect, 0);
						}

						//volume on -> off
						if (mouseX > volume_bt_rect.x && mouseY > volume_bt_rect.y &&
							mouseX < volume_bt_rect.x + volume_bt_rect.w && mouseY < volume_bt_rect.y + volume_bt_rect.h) {
							isVolumeOff = !isVolumeOff;
							Mix_PlayChannel(-1, SoundEffect, 0);
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
							Mix_PlayChannel(-1, SoundEffect, 0);
						}

						//exit game
						if (mouseX > home_rect.x && mouseY > home_rect.y &&
							mouseX < home_rect.x + home_rect.w && mouseY < home_rect.y + home_rect.h) {
							exit(1);
						}
					}
				
				
				
			}
		}

		

	}
}

void gameMain::Update() {
	timebar_rect.w = 540;
	count_ = 0;
	sec = 0;
	last_sec = 0;
	isChanged  = false;
}

void gameMain::Render() {
	SDL_RenderCopy(g_renderer, main_bg, NULL, NULL);
	

	

	if (cat_status == false) {
		//cat image 1
		SDL_Rect tmp_r;
		tmp_r.x = 0;
		tmp_r.y = 0;
		tmp_r.w = cat_rect.w;
		tmp_r.h = cat_rect.h;
		SDL_RenderCopy(g_renderer, cat, &cat_rect, &tmp_r);

	}
	else {
		//cat image 2
		SDL_Rect tmp_r1;
		tmp_r1.x = -540;
		tmp_r1.y = 0;
		tmp_r1.w = cat_rect.w;
		tmp_r1.h = cat_rect.h;
		SDL_RenderCopy(g_renderer, cat, &cat_rect, &tmp_r1);
	}


	{ //score text
		SDL_Rect tmp_r;
		tmp_r.x = g_window_margin + 80;
		tmp_r.y = 37;
		tmp_r.w = score_rect.w * 0.8;
		tmp_r.h = score_rect.h * 0.8;
		SDL_RenderCopy(g_renderer, score_text, &score_rect, &tmp_r);
	}




	if (isRule == true) {
		SDL_Rect tmp_r;
		tmp_r.x = 0;
		tmp_r.y = 0;
		tmp_r.w = rule_rect.w;
		tmp_r.h = rule_rect.h;
		SDL_RenderCopy(g_renderer, rule, &rule_rect, &tmp_r);
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

void gameMain::changePhaseToPlay() { 
	g_current_game_phase = PHASE_PLAYING;
	Mix_HaltMusic();
	Mix_PlayMusic(play_music, -1);

}