#include "Include.h"
#include "gameIntro.h"
#include "gameEndingClear.h"
#include "gameEndingGameover.h"
#include "gameMain.h"
#include "gamePlay.h"

SDL_Surface* sprite;
int step = 0;

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
Mix_Music* main_music;
Mix_Music* play_music;

SDL_Rect timebar_rect;
int stage;
int count_;
int sec; //play second
int limit_sec;
int last_sec;

bool isChanged;
bool isForcedQuit;
bool cat_status = false; //false: image1, true : image2
int org_score_int;
int plus_score_int;

string original_score;

int main(int argc, char* argv[]) {
	
	// Initializing SDL library
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	isChanged = false;

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
	game_phases.push_back(new gamePlay()); // PHASE_PLAYING
	game_phases.push_back(new gameEndingClear()); // PHASE_ENDING_CLEAR
   	game_phases.push_back(new gameEndingGameover()); // PHASE_ENDING_GAMEOVER
   	//game_phases.push_back(new gamePause());

	g_current_game_phase = PHASE_INTRO;

	

	g_last_time_ms = SDL_GetTicks();



	//cat animation timer
	Timer main_t;
	main_t.setInterval(1000);
	main_t.start();


	
	



	while (g_flag_running) {
		

		//write score to txt file
			string score;

			if (isForcedQuit == true) {
				
				plus_score_int = org_score_int;
				score = original_score;

				
			}
			else {
				score = std::to_string(plus_score_int);
				
			}
			//write to txt file

			ofstream ofs;
			ofs.open("../../res/testRes/scoreboard.txt");
			ofs << score;
			ofs.close();
		



		if (main_t.done()) {
			cat_status = !cat_status;
			//printf("1s elapsed\n");
		}
		
		int count = 0;

		Uint32 cur_time_ms = SDL_GetTicks();

		if (cur_time_ms - g_last_time_ms < 33)
			continue;

		game_phases[g_current_game_phase]->HandleEvents();
		game_phases[g_current_game_phase]->Update();
		game_phases[g_current_game_phase]->Render();

		g_last_time_ms = cur_time_ms;

		

		SDL_Delay(1);
		
	}

	// game_phases 배열의 동적 할당된 객체 삭제
	for (size_t i = 0; i < sizeof(game_phases) / sizeof(game_phases[0]); ++i) {
		delete game_phases[i];
	}
	ClearGame();

	return 0;
}
