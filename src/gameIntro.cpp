#include "gameIntro.h"

bool intro_reset = false;

gameIntro::gameIntro() {
	// Load cartoons
	cartoon.resize(cartoon_cut);

	cartoon[0].surface = IMG_Load("../../res/intro_cartoon/intro_cartoon1.png");
	cartoon[1].surface = IMG_Load("../../res/intro_cartoon/intro_cartoon2.png");
	cartoon[2].surface = IMG_Load("../../res/intro_cartoon/intro_cartoon3.png");
	cartoon[3].surface = IMG_Load("../../res/intro_cartoon/intro_cartoon4.png");
	cartoon[4].surface = IMG_Load("../../res/intro_cartoon/intro_cartoon5.png");
	cartoon[5].surface = IMG_Load("../../res/intro_cartoon/intro_cartoon6.png");

	// Cartoon surfaces
	for (int i = 0; i < cartoon_cut; i++) {
		cartoon[i].texture = SDL_CreateTextureFromSurface(g_renderer, cartoon[i].surface);
		SDL_FreeSurface(cartoon[i].surface);
		SDL_QueryTexture(cartoon[i].texture, NULL, NULL, &cartoon[i].source_rect.w, &cartoon[i].source_rect.h);
		cartoon[i].source_rect.x = cartoon[i].source_rect.y = 0;
		
		// Set cartoon destination rect
		cartoon[i].destination_rect.x = 0;
		cartoon[i].destination_rect.y = 0;
		cartoon[i].destination_rect.w = cartoon[i].source_rect.w;
		cartoon[i].destination_rect.h = cartoon[i].source_rect.h;
	}

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
	//std::cout << g_current_game_phase << std::endl;

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
				g_current_game_phase = PHASE_MAIN;
				Mix_HaltMusic();
			}
			// Any keys (except spacebar)
			else {
				if (is_cartoon_finished) {
					g_current_game_phase = PHASE_MAIN;
					Mix_HaltMusic();
				}
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
	* 3. Use these functions selectively for next cartoons
	*/

	if (intro_reset) {
		for (int i = 0; i < cartoon_cut; i++) {
			cartoon[i].fade_value = 0;
			cartoon[i].delay_elapsed_time = 0;
			cartoon[i].is_fade_finished = false;
			cartoon[i].is_delay_finished = false;
		}
		intro_reset = false;
	}

	cartoon[0].FadeIn(0.5);
	if (cartoon[0].is_fade_finished)
		cartoon[0].Delay(3);

	if (cartoon[0].is_delay_finished)
		cartoon[1].FadeIn(0.5);
	if (cartoon[1].is_fade_finished)
		cartoon[1].Delay(1);

	if (cartoon[1].is_delay_finished)
		cartoon[2].FadeIn(0.5);
	if (cartoon[2].is_fade_finished)
		cartoon[2].Delay(2);

	if (cartoon[2].is_delay_finished)
		cartoon[3].FadeIn(0.5);
	if (cartoon[3].is_fade_finished)
		cartoon[3].Delay(5);

	if (cartoon[3].is_delay_finished)
		cartoon[4].FadeIn(0.5);
	if (cartoon[4].is_fade_finished)
		cartoon[4].Delay(2);

	if (cartoon[4].is_delay_finished)
		cartoon[5].FadeIn(0.5);

	// Prevent skipping cartoon
	if (cartoon[5].is_fade_finished)
		is_cartoon_finished = true;

	// Draw to the screen
	SDL_RenderPresent(g_renderer);
}