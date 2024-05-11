#include "Include.h"
#include "gameIntro.h"
#include "gameEndingClear.h"
#include "gameEndingGameover.h"
#include "gameMain.h"
//#include "gamePlay.h"

SDL_Window* g_window;
SDL_Renderer* g_renderer;
bool g_flag_running;
Uint32 g_last_time_ms;

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
	
	//! 게임 페이즈 객체를 담는 벡터(순서대로 배치해야 ENUM과 매칭됨)
	/*! ENUM 순서는 
	PHASE_INTRO 		/ PHASE_MAIN 			/ PHASE_PLAYING
	PHASE_ENDING_CLEAR / PHASE_ENDING_GAMEOVER / PHASE_PAUSE
	*/
	std::vector<gameClass*> game_phases;
    game_phases.push_back(new gameIntro()); // PHASE_INTRO
	game_phases.push_back(new gameMain()); // PHASE_MAIN
	//game_phases.push_back(new gamePlay()); // PHASE_PLAYING
    game_phases.push_back(new gameEndingClear()); // PHASE_ENDING_CLEAR
    game_phases.push_back(new gameEndingGameover()); // PHASE_ENDING_GAMEOVER
    // game_phases.push_back(new gamePause());

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

	// game_phases 배열의 동적 할당된 객체 삭제
	for (size_t i = 0; i < sizeof(game_phases) / sizeof(game_phases[0]); ++i) {
		delete game_phases[i];
	}
	ClearGame();

	return 0;
}