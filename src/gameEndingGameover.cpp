#include "gameEndingGameover.h"

gameEndingGameover::gameEndingGameover() {
	// For Texture
	surface = IMG_Load("../../res/testRes/testGameover.jpg");
	texture = SDL_CreateTextureFromSurface(g_renderer, surface);
	SDL_FreeSurface(surface);

	SDL_QueryTexture(texture, NULL, NULL, &source_rect.w, &source_rect.h);
	destination_rect.x = destination_rect.x = 0;
	destination_rect.y = destination_rect.y = 0;
	destination_rect.w = source_rect.w;
	destination_rect.h = source_rect.h;

	// For BGM
	gameover_music = Mix_LoadMUS("../../res/testRes/testBGM3.mp3");
	if (gameover_music == 0) {
		printf("Couldn't load the wav: %s\n", Mix_GetError());
	}
}

gameEndingGameover::~gameEndingGameover() {
	Mix_FreeMusic(gameover_music);
	SDL_DestroyTexture(texture);
}

void gameEndingGameover::HandleEvents() {
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
				g_current_game_phase = PHASE_INTRO;
				Mix_HaltMusic();
				intro_reset = true;
			}
			// Any keys (except spacebar)
			else {
				g_current_game_phase = PHASE_MAIN;
				Mix_HaltMusic();
			}
			break;

		default:
			break;
		}
	}
}

void gameEndingGameover::Update() {
}

void gameEndingGameover::Render() {
	SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
	SDL_RenderClear(g_renderer); // clear the renderer to the draw color
	SDL_RenderCopy(g_renderer, texture, &source_rect, &destination_rect);
	SDL_RenderPresent(g_renderer); // draw to the screen
}