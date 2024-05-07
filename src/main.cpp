#include "Include.h"
#include "gameIntro.h"
#include "gameEndingClear.h"
#include "gameEndingGameover.h"
#include "MainScreen.h"

SDL_Window* g_window;
SDL_Renderer* g_renderer;
bool g_flag_running;
Uint32 g_last_time_ms;

// Window Margin 20px
const int g_window_margin = 20; 

// Game Phases
int g_current_game_phase;

// Game Music
Mix_Music* intro_music;
Mix_Music* clear_music;
Mix_Music* gameover_music;

int main(int argc, char* argv[]) {
	// Initializing SDL library
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		std::cout << "Mix_OpenAudio " << Mix_GetError() << std::endl;
		exit(1);
	}

	g_window = SDL_CreateWindow("Operation Kitsch Catch", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	g_renderer = SDL_CreateRenderer(g_window, -1, 0);

	InitGame();
	
	gameClass* game_phases[4];
	game_phases[0] = new gameIntro();
	game_phases[1] = new gameEndingClear();
	game_phases[2] = new gameEndingGameover();
	game_phases[3] = new MainScreen();
	//game_phases[2] = new gamePlay();
	//game_phases[5] = new gamePause();

	g_current_game_phase = PHASE_INTRO;

	g_last_time_ms = SDL_GetTicks();

	while (g_flag_running) {

		Uint32 cur_time_ms = SDL_GetTicks();

		if (cur_time_ms - g_last_time_ms < 33)
			continue;

		game_phases[g_current_game_phase]->HandleEvents();
		game_phases[g_current_game_phase]->Update();
		game_phases[g_current_game_phase]->Render();

		g_last_time_ms = cur_time_ms;
	}

	delete game_phases[0];
	delete game_phases[1];
	delete game_phases[2];
	delete game_phases[3];

	ClearGame();

	return 0;
}