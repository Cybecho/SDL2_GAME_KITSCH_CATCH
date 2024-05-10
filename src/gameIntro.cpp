#include "gameIntro.h"

gameIntro::gameIntro() {
	// Load cartoons
	cartoon.resize(cartoon_cut);

	cartoon[0].surface = IMG_Load("../../res/testRes/testCartoon1.jpg");
	cartoon[1].surface = IMG_Load("../../res/testRes/testCartoon2.jpg");
	cartoon[2].surface = IMG_Load("../../res/testRes/testCartoon3.jpg");

	// Cartoon surfaces
	for (int i = 0; i < cartoon_cut; i++) {
		cartoon[i].texture = SDL_CreateTextureFromSurface(g_renderer, cartoon[i].surface);
		SDL_FreeSurface(cartoon[i].surface);
		SDL_QueryTexture(cartoon[i].texture, NULL, NULL, &cartoon[i].source_rect.w, &cartoon[i].source_rect.h);
		cartoon[i].source_rect.x = cartoon[i].source_rect.y = 0;
		
		// Set cartoon x-position
		cartoon[i].destination_rect.x = g_window_margin;
		
		// Set cartoon width and height
		cartoon[i].destination_rect.w = WINDOW_WIDTH - 2 * g_window_margin;
		cartoon[i].destination_rect.h = 280;
	}
	
	// Set cartoon y-position
	cartoon[0].destination_rect.y = g_window_margin;
	cartoon[1].destination_rect.y = cartoon[0].destination_rect.y + cartoon[0].destination_rect.h + g_window_margin;
	cartoon[2].destination_rect.y = cartoon[1].destination_rect.y + cartoon[1].destination_rect.h + g_window_margin;

	// Init Sound
	intro_music = Mix_LoadMUS("../../res/testRes/testBGM1.mp3");
	if (intro_music == 0) {
		printf("Couldn't load the wav: %s\n", Mix_GetError());
	}
	Mix_VolumeMusic(128);
	Mix_PlayMusic(intro_music, -1);
}

gameIntro::~gameIntro() {
	Mix_FreeMusic(intro_music);
	for (int i = 0;i < cartoon_cut;i++) {
		SDL_DestroyTexture(cartoon[i].texture);
	}
}

void gameIntro::HandleEvents() {
	// Debug Print
	//std::cout << " " << std::endl

	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			// Spacebar (cheat key)
			if (event.key.keysym.sym == SDLK_SPACE) {
				//g_current_game_phase = PHASE_MAIN;
				g_current_game_phase = PHASE_MAIN;
				Mix_HaltMusic();
			}
			// Any keys (except spacebar)
			else {
				//g_current_game_phase = PHASE_MAIN;
				g_current_game_phase = PHASE_MAIN;
				Mix_PlayMusic(clear_music, -1);
			}
			break;

		default:
			break;
		}
	}
}

void gameIntro::Update() {
}

void gameIntro::Render() {
	// BG
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	SDL_RenderClear(g_renderer);

	/*
	* Cartoon functions
	* 1. We can't use Sleep() and SDL_Delay(), so use these functions
	* 2. When FadeIn() function finished, "is_fade_finished" = true
	*	 When Delay() function finished, "is_delay_finished" = true
	* 3. Use these Functions selectively for next cartoons
	*/

	cartoon[0].FadeIn(1);
	if (cartoon[0].is_fade_finished) {
		cartoon[0].Delay(3);
	}
	if (cartoon[0].is_delay_finished) {
		cartoon[2].FadeIn(3);
	}
	if (cartoon[2].is_fade_finished) {
		cartoon[1].FadeIn(4);
	}

	// Draw to the screen
	SDL_RenderPresent(g_renderer);
}