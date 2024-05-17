#include "gameMain.h"

SDL_Texture* exp_text; //"EXP" text
SDL_Texture* score_text; //get score from txt file




gameMain::gameMain() {
	
	
	


	// BG
	SDL_Surface* bg_surface = IMG_Load("../../res/main page/main_background.png");
	main_bg = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);


	playBT_dir_rect.x = 140;
	playBT_dir_rect.y = 850;
	playBT_dir_rect.w = 260;
	playBT_dir_rect.h = 90;



	

	//cat
	SDL_Surface* cat_surface = IMG_Load("../../res/main page/main_sprite.png");
	cat = SDL_CreateTextureFromSurface(g_renderer, cat_surface);

	cat_rect.x = 0;
	cat_rect.y = 0;
	cat_rect.w = cat_surface->w;
	cat_rect.h = cat_surface->h;

	SDL_FreeSurface(cat_surface);


	/*

	//interaction button
	SDL_Surface* inter_surface = IMG_Load("../../res/testRes/testMainInteractionBT.png");
	inter_bt = SDL_CreateTextureFromSurface(g_renderer, inter_surface);

	interBT_rect.x = 0;
	interBT_rect.y = 0;
	interBT_rect.w = inter_surface->w;
	interBT_rect.h = inter_surface->h;

	SDL_FreeSurface(inter_surface);




	//play button
	SDL_Surface* play_surface = IMG_Load("../../res/testRes/testMainPlayBT.png");
	play_bt = SDL_CreateTextureFromSurface(g_renderer, play_surface);

	playBT_rect.x = 0;
	playBT_rect.y = 0;
	playBT_rect.w = play_surface->w;
	playBT_rect.h = play_surface->h;

	SDL_FreeSurface(play_surface);




	//setting button
	SDL_Surface* setting_surface = IMG_Load("../../res/testRes/testMainSettingBT.png");
	setting_bt = SDL_CreateTextureFromSurface(g_renderer, setting_surface);

	settingBT_rect.x = 0;
	settingBT_rect.y = 0;
	settingBT_rect.w = setting_surface->w;
	settingBT_rect.h = setting_surface->h;

	SDL_FreeSurface(setting_surface);



	//text

	{ //exp text
		TTF_Font* font = TTF_OpenFont("../../res/testRes/Galmuri14.ttf", 30);
		SDL_Color white = { 255,255,255,0 };
		SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(font, "EXP : ", white);

		exp_rect.x = 0;
		exp_rect.y = 0;
		exp_rect.w = tmp_surface->w;
		exp_rect.h = tmp_surface->h;

		exp_text = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
		SDL_FreeSurface(tmp_surface);
		TTF_CloseFont(font);

	}
	*/

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



	//music
	main_music = Mix_LoadMUS("../../res/testRes/testBGM2.mp3");
	if (main_music == 0) {
		std::cout << "Mix_LoadMUS(\"testBGM2.mp3\"): " << Mix_GetError() << std::endl;
	}
	//Mix_VolumeMusic(128);
	Mix_PlayMusic(main_music, -1);


	//sound effect(play button)
	SoundEffect = Mix_LoadWAV("../../res/testRes/testSound.mp3");
}

gameMain::~gameMain() {
	Mix_FreeMusic(main_music);
	Mix_FreeChunk(SoundEffect);
	//SDL_DestroyTexture(exp_text);
	SDL_DestroyTexture(score_text);
	SDL_DestroyTexture(main_bg);
	//SDL_DestroyTexture(cat);
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
				if (mouseX > playBT_dir_rect.x && mouseY > playBT_dir_rect.y && 
					mouseX < playBT_dir_rect.x + playBT_dir_rect.w && mouseY < playBT_dir_rect.y + playBT_dir_rect.h) {
					changePhaseToPlay();
					Mix_PlayChannel(-1, SoundEffect, 0);
				}
			}
		}

	}
}

void gameMain::Update() {
	
}

void gameMain::Render() {
	SDL_RenderCopy(g_renderer, main_bg, NULL, NULL);
	/*
	{ //play button
		playBT_dir_rect.x = g_window_margin;
		playBT_dir_rect.y = 800;
		playBT_dir_rect.w = playBT_rect.w;
		playBT_dir_rect.h = playBT_rect.h;
		SDL_RenderCopy(g_renderer, play_bt, &playBT_rect, &playBT_dir_rect);
	}
	
	{ //interaction button
		SDL_Rect tmp_r;
		tmp_r.x = g_window_margin;
		tmp_r.y = 120;
		tmp_r.w = interBT_rect.w;
		tmp_r.h = interBT_rect.h;
		SDL_RenderCopy(g_renderer, inter_bt, &interBT_rect, &tmp_r);
	}

	{ //setting button
		SDL_Rect tmp_r;
		tmp_r.x = 440;
		tmp_r.y = 20;
		tmp_r.w = settingBT_rect.w;
		tmp_r.h = settingBT_rect.h;
		SDL_RenderCopy(g_renderer, setting_bt, &settingBT_rect, &tmp_r);
	}
	*/

	




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
	
		
	
	
	
	

	/*

	{ //EXP text
		SDL_Rect tmp_r;
		tmp_r.x = g_window_margin;
		tmp_r.y = g_window_margin * 2;
		tmp_r.w = exp_rect.w;
		tmp_r.h = exp_rect.h;
		SDL_RenderCopy(g_renderer, exp_text, &exp_rect, &tmp_r);
	}
	*/
	{ //score text
		SDL_Rect tmp_r;
		tmp_r.x = g_window_margin + 80;
		tmp_r.y = 37;
		tmp_r.w = score_rect.w * 0.8;
		tmp_r.h = score_rect.h * 0.8;
		SDL_RenderCopy(g_renderer, score_text, &score_rect, &tmp_r);
	}

	SDL_RenderPresent(g_renderer);
}

void gameMain::changePhaseToPlay() { 
	g_current_game_phase = PHASE_PLAYING;
	Mix_HaltMusic();
	Mix_PlayMusic(play_music, -1);

}
