#include "gamePlay.h"

extern SDL_Texture* exp_text; //"EXP" text
extern SDL_Texture* score_text; //get score from txt file

gamePlay::gamePlay() {

	// BG
	SDL_Surface* bg_surface = IMG_Load("../../res/testRes/testPlayBG.png");
	play_bg = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);




	//cat
	SDL_Surface* cat_surface = IMG_Load("../../res/testRes/testMainCat.png");
	cat = SDL_CreateTextureFromSurface(g_renderer, cat_surface);

	cat_rect.x = 0;
	cat_rect.y = 0;
	cat_rect.w = cat_surface->w;
	cat_rect.h = cat_surface->h;

	SDL_FreeSurface(cat_surface);


	//setting button
	SDL_Surface* setting_surface = IMG_Load("../../res/testRes/testMainSettingBT.png");
	setting_bt = SDL_CreateTextureFromSurface(g_renderer, setting_surface);

	settingBT_rect.x = 0;
	settingBT_rect.y = 0;
	settingBT_rect.w = setting_surface->w;
	settingBT_rect.h = setting_surface->h;

	SDL_FreeSurface(setting_surface);


	//time bar
	SDL_Surface* timebar_surface = IMG_Load("../../res/testRes/testPlayTimebar.png");
	timebar_bg = SDL_CreateTextureFromSurface(g_renderer, timebar_surface);

	timebar_bg_rect.x = 0;
	timebar_bg_rect.y = 0;
	timebar_bg_rect.w = timebar_surface->w;
	timebar_bg_rect.h = timebar_surface->h;

	SDL_FreeSurface(timebar_surface);


	//playground
	SDL_Surface* playground_surface = IMG_Load("../../res/testRes/testPlayground.png");
	playground_img = SDL_CreateTextureFromSurface(g_renderer, playground_surface);

	playground_rect.x = 0;
	playground_rect.y = 0;
	playground_rect.w = playground_surface->w;
	playground_rect.h = playground_surface->h;

	SDL_FreeSurface(playground_surface);

	//stack
	SDL_Surface* stack_surface = IMG_Load("../../res/testRes/testPlayStack.png");
	stack_img = SDL_CreateTextureFromSurface(g_renderer, stack_surface);

	stack_rect.x = 0;
	stack_rect.y = 0;
	stack_rect.w = stack_surface->w;
	stack_rect.h = stack_surface->h;

	SDL_FreeSurface(stack_surface);


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


	{ //score text
		ifstream file("../../res/testRes/scoreboard.txt"); //read scoreboard
		getline(file, score);

		TTF_Font* font = TTF_OpenFont("../../res/testRes/Galmuri14.ttf", 30);
		SDL_Color white = { 255,255,255,0 };
		SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(font, score.c_str(), white);
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
}

gamePlay::~gamePlay() {
	Mix_FreeMusic(play_music);
	SDL_DestroyTexture(exp_text);
	SDL_DestroyTexture(score_text);
	SDL_DestroyTexture(play_bg);
	SDL_DestroyTexture(cat);
	SDL_DestroyTexture(setting_bt);
	SDL_DestroyTexture(timebar_bg);
	SDL_DestroyTexture(stack_img);
	SDL_DestroyTexture(playground_img);
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
				//changePhaseToEnding();
			}
			else {
				Mix_PlayMusic(play_music, -1);
			}break;

		default: break;
		}

	}
}

void gamePlay::Update() {
	
	timebar_rect.w = timebar_bg_dir_rect.w / 4;
}

void gamePlay::Render() {
	SDL_RenderCopy(g_renderer, play_bg, NULL, NULL);


	






	{ //setting button
		SDL_Rect tmp_r;
		tmp_r.x = 440;
		tmp_r.y = g_window_margin;
		tmp_r.w = settingBT_rect.w;
		tmp_r.h = settingBT_rect.h;
		SDL_RenderCopy(g_renderer, setting_bt, &settingBT_rect, &tmp_r);
	}

	{ //cat image
		SDL_Rect tmp_r;
		tmp_r.x = 30;
		tmp_r.y = 90;
		tmp_r.w = cat_rect.w;
		tmp_r.h = cat_rect.h;
		SDL_RenderCopy(g_renderer, cat, &cat_rect, &tmp_r);
	}

	{ //timebar bg
		timebar_bg_dir_rect.x = g_window_margin;
		timebar_bg_dir_rect.y = 405;
		timebar_bg_dir_rect.w = timebar_bg_rect.w;
		timebar_bg_dir_rect.h = timebar_bg_rect.h;
		SDL_RenderCopy(g_renderer, timebar_bg, &timebar_bg_rect, &timebar_bg_dir_rect);
	}

	{ //timebar
		timebar_rect.x = timebar_bg_dir_rect.x;
		timebar_rect.y = timebar_bg_dir_rect.y;
		//timebar_rect.w = timebar_bg_dir_rect.w;
		timebar_rect.h = timebar_bg_dir_rect.h;

		SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255); //set time bar red
		SDL_RenderFillRect(g_renderer, &timebar_rect);


	}

	{//stack img
		SDL_Rect tmp_r;
		tmp_r.x = g_window_margin;
		tmp_r.y = 450;
		tmp_r.w = stack_rect.w;
		tmp_r.h = stack_rect.h;
		SDL_RenderCopy(g_renderer, stack_img, &stack_rect, &tmp_r);
	}

	{ //playground img
		SDL_Rect tmp_r;
		tmp_r.x = g_window_margin;
		tmp_r.y = 540;
		tmp_r.w = playground_rect.w;
		tmp_r.h = playground_rect.h;
		SDL_RenderCopy(g_renderer, playground_img, &playground_rect, &tmp_r);
	}


	{ //EXP text
		SDL_Rect tmp_r;
		tmp_r.x = g_window_margin;
		tmp_r.y = g_window_margin * 2;
		tmp_r.w = exp_rect.w;
		tmp_r.h = exp_rect.h;
		SDL_RenderCopy(g_renderer, exp_text, &exp_rect, &tmp_r);
	}

	{ //score text
		SDL_Rect tmp_r;
		tmp_r.x = g_window_margin + exp_rect.w;
		tmp_r.y = g_window_margin * 2;
		tmp_r.w = score_rect.w;
		tmp_r.h = score_rect.h;
		SDL_RenderCopy(g_renderer, score_text, &score_rect, &tmp_r);
	}

	SDL_RenderPresent(g_renderer);
}

void gamePlay::changePhaseToEnding() {
	g_current_game_phase = PHASE_ENDING_CLEAR;
	Mix_HaltMusic();
	Mix_PlayMusic(clear_music, -1);

}
