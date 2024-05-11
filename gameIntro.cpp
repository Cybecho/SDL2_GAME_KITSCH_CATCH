#include "gameIntro.h"

gameIntro::gameIntro() {
	// ��ȭ �ε�
	cartoon.resize(cartoon_cut);

	cartoon[0].surface = IMG_Load("../../res/testCartoon1.jpg");
	cartoon[1].surface = IMG_Load("../../res/testCartoon2.jpg");
	cartoon[2].surface = IMG_Load("../../res/testCartoon3.jpg");

	// ��ȭ Surface ó��
	for (int i = 0; i < cartoon_cut; i++) {
		cartoon[i].texture = SDL_CreateTextureFromSurface(g_renderer, cartoon[i].surface);
		SDL_FreeSurface(cartoon[i].surface);
		SDL_QueryTexture(cartoon[i].texture, NULL, NULL, &cartoon[i].source_rect.w, &cartoon[i].source_rect.h);
		cartoon[i].source_rect.x = cartoon[i].source_rect.y = 0;
		
		// ��ȭ x��ǥ: margin ��ŭ ����߸�
		cartoon[i].destination_rect.x = g_window_margin;
		
		// ��ȭ w & h: ������ ũ�� - 2 * margin
		cartoon[i].destination_rect.w = WINDOW_WIDTH - 2 * g_window_margin;
		cartoon[i].destination_rect.h = 280;
	}
	
	// ��ȭ y��ǥ ����
	cartoon[0].destination_rect.y = g_window_margin;
	cartoon[1].destination_rect.y = cartoon[0].destination_rect.y + cartoon[0].destination_rect.h + g_window_margin;
	cartoon[2].destination_rect.y = cartoon[1].destination_rect.y + cartoon[1].destination_rect.h + g_window_margin;

	// Init Sound
	intro_music = Mix_LoadMUS("../../res/testBGM1.mp3");
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
	// �׽�Ʈ�� print
	//std::cout << cartoon[1].fade_delta<<" " << cartoon[1].fade_alpha << " " << cartoon[1].fade_value << std::endl;
	
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			// �����̽��� ������ ��(ġƮŰ)
			if (event.key.keysym.sym == SDLK_SPACE) {
				//g_current_game_phase = PHASE_MAIN;
				//g_current_game_phase = PHASE_ENDINGCLEAR;
				Mix_HaltMusic();
			}
			// �����̽��ٸ� ������ �ƹ�Ű ������ ��
			else {
				//g_current_game_phase = PHASE_MAIN;
				//g_current_game_phase = PHASE_ENDINGCLEAR;
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

	/** ��ȭ
	* 1. SDL���� ������ Sleep()�̳� SDL_Delay() ���� ���� �����
	* 2. FadeIn()�� ������ is_fade_finished = true�� �ٲ�
	*	 Delay()�� ������ is_delay_finished = true�� �ٲ�
	* 3. ���� ��ȭ�� ����� FadeIn �Ŀ� �Ұ���, Delay �Ŀ� �Ұ�����
	*	 ���ǹ��� �̿��Ͽ� ���������� �ۼ�
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