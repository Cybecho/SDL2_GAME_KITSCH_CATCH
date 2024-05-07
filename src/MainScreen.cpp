#include "MainScreen.h"

int score;
SDL_Texture* main_bg;
SDL_Texture* cat;
SDL_Texture* inter_bt;
SDL_Texture* play_bt;
SDL_Texture* setting_bt;

SDL_Rect playBT_rect;
SDL_Rect cat_rect;
SDL_Rect interBT_rect;
SDL_Rect settingBT_rect;

//text
SDL_Texture* exp_text; //"경험치" 글씨
SDL_Texture* score_text; //점수(경험치)
SDL_Rect exp_rect;
SDL_Rect score_rect;

MainScreen::MainScreen() {
	score = 0;
	// BG
	SDL_Surface* bg_surface = IMG_Load("../../Resources/testMainBG.png");
	main_bg = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);




	//cat
	SDL_Surface* cat_surface = IMG_Load("../../Resources/testMainCat.png");
	cat = SDL_CreateTextureFromSurface(g_renderer, cat_surface);

	cat_rect.x = 0;
	cat_rect.y = 0;
	cat_rect.w = cat_surface->w;
	cat_rect.h = cat_surface->h;

	SDL_FreeSurface(cat_surface);




	//상호작용버튼
	SDL_Surface* inter_surface = IMG_Load("../../Resources/testMainInteractionBT.png");
	inter_bt = SDL_CreateTextureFromSurface(g_renderer, inter_surface);

	interBT_rect.x = 0;
	interBT_rect.y = 0;
	interBT_rect.w = inter_surface->w;
	interBT_rect.h = inter_surface->h;

	SDL_FreeSurface(inter_surface);




	//플레이버튼
	SDL_Surface* play_surface = IMG_Load("../../Resources/testMainPlayBT.png");
	play_bt = SDL_CreateTextureFromSurface(g_renderer, play_surface);

	playBT_rect.x = 0;
	playBT_rect.y = 0;
	playBT_rect.w = play_surface->w;
	playBT_rect.h = play_surface->h;

	SDL_FreeSurface(play_surface);




	//설정버튼
	SDL_Surface* setting_surface = IMG_Load("../../Resources/testMainSettingBT.png");
	setting_bt = SDL_CreateTextureFromSurface(g_renderer, setting_surface);

	settingBT_rect.x = 0;
	settingBT_rect.y = 0;
	settingBT_rect.w = setting_surface->w;
	settingBT_rect.h = setting_surface->h;

	SDL_FreeSurface(setting_surface);



	//text

	{ //exp text
		TTF_Font* font = TTF_OpenFont("../../Resources/Galmuri14.ttf", 30);
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
		TTF_Font* font = TTF_OpenFont("../../Resources/Galmuri14.ttf", 30);
		SDL_Color white = { 255,255,255,0 };
		SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(font, std::to_string(score).c_str(), white);

		score_rect.x = 0;
		score_rect.y = 0;
		score_rect.w = tmp_surface->w;
		score_rect.h = tmp_surface->h;

		score_text = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
		SDL_FreeSurface(tmp_surface);
		TTF_CloseFont(font);

	}

}

MainScreen::~MainScreen() {

}

void MainScreen::HandleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_SPACE) {
				//g_current_game_phase = PHASE_PLAY;
			}break;

		default: break;
		}

	}
}
 
void MainScreen::Update() {
}

void MainScreen::Render() {
	SDL_RenderCopy(g_renderer, main_bg, NULL, NULL);
	
	{ //플레이버튼
		SDL_Rect tmp_r;
		tmp_r.x = g_window_margin;
		tmp_r.y = 800;
		tmp_r.w = playBT_rect.w;
		tmp_r.h = playBT_rect.h;
		SDL_RenderCopy(g_renderer, play_bt, &playBT_rect, &tmp_r);
	}

	{ //상호작용버튼
		SDL_Rect tmp_r;
		tmp_r.x = g_window_margin;
		tmp_r.y = 120;
		tmp_r.w = interBT_rect.w;
		tmp_r.h = interBT_rect.h;
		SDL_RenderCopy(g_renderer, inter_bt, &interBT_rect, &tmp_r);
	}

	{ //설정버튼
		SDL_Rect tmp_r;
		tmp_r.x = 440;
		tmp_r.y = 20;
		tmp_r.w = settingBT_rect.w;
		tmp_r.h = settingBT_rect.h;
		SDL_RenderCopy(g_renderer, setting_bt, &settingBT_rect, &tmp_r);
	}

	{ //고양이사진
		SDL_Rect tmp_r;
		tmp_r.x = 70;
		tmp_r.y = 400;
		tmp_r.w = cat_rect.w;
		tmp_r.h = cat_rect.h;
		SDL_RenderCopy(g_renderer, cat, &cat_rect, &tmp_r);
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
