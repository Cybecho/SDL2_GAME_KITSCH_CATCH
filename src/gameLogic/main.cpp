#include "GameLogic.h"

SDL_Window *g_window;
SDL_Renderer *g_renderer;
bool g_flag_running;
Uint32 g_last_time_ms;
Uint32 g_frame_per_sec = 30;

int main(int argc, char *argv[])
{

	// Initializing SDL library
	SDL_Init(SDL_INIT_EVERYTHING);

	TTF_Init();

	// 오디오 초기화
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		cout << "Mix_OpenAudio " << Mix_GetError() << endl;
		exit(1);
	}

	g_window = SDL_CreateWindow("201921408 SBU", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	g_renderer = SDL_CreateRenderer(g_window, -1, 0);

	InitGame();

	g_last_time_ms = SDL_GetTicks();

	while (g_flag_running)
	{

		Uint32 cur_time_ms = SDL_GetTicks();

		if (cur_time_ms - g_last_time_ms < (1000 / g_frame_per_sec))
			continue;

		HandleEvents();
		Update();
		Render();

		g_last_time_ms = cur_time_ms;
	}

	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);

	ClearGame();
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();

	return 0;
}