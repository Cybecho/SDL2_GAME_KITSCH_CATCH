#include "gameIntro.h"

gameIntro::gameIntro() {
	// 만화 로딩
	cartoon.resize(cartoon_cut);

	cartoon[0].surface = IMG_Load("../../Resources/testCartoon1.jpg");
	cartoon[1].surface = IMG_Load("../../Resources/testCartoon2.jpg");
	cartoon[2].surface = IMG_Load("../../Resources/testCartoon3.jpg");

	// 만화 Surface 처리
	for (int i = 0; i < cartoon_cut; i++) {
		cartoon[i].texture = SDL_CreateTextureFromSurface(g_renderer, cartoon[i].surface);
		SDL_FreeSurface(cartoon[i].surface);
		SDL_QueryTexture(cartoon[i].texture, NULL, NULL, &cartoon[i].source_rect.w, &cartoon[i].source_rect.h);
		cartoon[i].source_rect.x = cartoon[i].source_rect.y = 0;
		
		// 만화 x좌표: margin 만큼 떨어뜨림
		cartoon[i].destination_rect.x = g_window_margin;
		
		// 만화 w & h: 윈도우 크기 - 2 * margin
		cartoon[i].destination_rect.w = WINDOW_WIDTH - 2 * g_window_margin;
		cartoon[i].destination_rect.h = 280;
	}
	
	// 만화 y좌표 지정
	cartoon[0].destination_rect.y = g_window_margin;
	cartoon[1].destination_rect.y = cartoon[0].destination_rect.y + cartoon[0].destination_rect.h + g_window_margin;
	cartoon[2].destination_rect.y = cartoon[1].destination_rect.y + cartoon[1].destination_rect.h + g_window_margin;

	// Init Sound
	intro_music = Mix_LoadMUS("../../Resources/testBGM1.mp3");
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
	// 테스트용 print
	//std::cout << cartoon[1].fade_delta<<" " << cartoon[1].fade_alpha << " " << cartoon[1].fade_value << std::endl;
	
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			// 스페이스바 눌렀을 때(치트키)
			if (event.key.keysym.sym == SDLK_SPACE) {
				//g_current_game_phase = PHASE_MAIN;
				g_current_game_phase = PHASE_ENDINGCLEAR;
				Mix_HaltMusic();
			}
			// 스페이스바를 제외한 아무키 눌렀을 때
			else {
				//g_current_game_phase = PHASE_MAIN;
				g_current_game_phase = PHASE_ENDINGCLEAR;
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

	/** 만화
	* 1. SDL게임 구조상 Sleep()이나 SDL_Delay() 쓰면 게임 먹통됨
	* 2. FadeIn()이 끝나면 is_fade_finished = true로 바뀜
	*	 Delay()가 끝나면 is_delay_finished = true로 바뀜
	* 3. 다음 만화컷 출력을 FadeIn 후에 할건지, Delay 후에 할건지를
	*	 조건문을 이용하여 선택적으로 작성
	**/
	
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

	// draw to the screen
	SDL_RenderPresent(g_renderer);
}