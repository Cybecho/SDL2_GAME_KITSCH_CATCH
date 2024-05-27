#include "gamePlay.h"

extern SDL_Texture* exp_text; //"EXP" text


gamePlay::gamePlay() {

	isForcedQuit = false;
	isChanged = false;
	sec = 0;
	count_ = 0;
	stage = 1;


	//isChanged = false;
	isSetting = false;
	isVolumeOff = false;

	loadIMGs();

	loadSounds();

	//! ************************** gameLogic **************************
	cout << "Level : " << m_gameLogic.getLevel() << endl;
}

gamePlay::~gamePlay() {
	Mix_FreeMusic(play_music);
	Mix_FreeChunk(setting_SoundEffect);
	SDL_DestroyTexture(score_text2);
	SDL_DestroyTexture(play_bg);
	SDL_DestroyTexture(cat);
	SDL_DestroyTexture(cat2);
	SDL_DestroyTexture(setting);
	SDL_Quit();
	TTF_Quit();
}

void gamePlay::HandleEvents() {
	//SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			//~ space 눌렀을때 ending으로 넘어가기
			if (event.key.keysym.sym == SDLK_SPACE) {
				isChanged = true;
				isForcedQuit = true;
				SDL_Delay(33);
				changePhase(PHASE_ENDING_CLEAR);
			}
			//~ n 눌렀을때 gameLogic 초기화 (LoadMahjongFromCSV() 불러오는 역할임)
			else if (event.key.keysym.sym == SDLK_n) {
				if (m_gameLogic.getMaxLevel() > m_gameLogic.getLevel())
				{
					increaseLevelLogic();
					loadMahjongBlocks();
				}
				else
				{
					cout << "MAX LEVEL!" << endl;
				}
			}
			else {
				Mix_PlayMusic(play_music, -1);
			}

		default: break;
		}


		MouseButtonEvents();
	}
	//! ************************** gameLogic **************************
	m_gameLogic.HandleEvents(); //~ 게임로직 이벤트 처리 함수 실행
}

void gamePlay::Update() {

	//! ************************** gameLogic **************************
	checkGameStatus();				//~ 게임 상태 체크
	m_gameLogic.LoadMahjongBlocksIfEmpty(m_gameLogic.getLevel());
	m_gameLogic.RemoveSameTypeBlocks();
	m_gameLogic.AlignStackBlocks();
	m_gameLogic.UpdateVectorBlocks();
	m_gameLogic.UpdateStackBlocks();
	m_gameLogic.UpdateBonks();
	m_gameLogic.printStatusChange();//~ 게임 상태 출력
	checkAndLoadMahjongBlocks();	//~ 맞춰야 할 블록 체크 및 로드
	//! ************************** ********* **************************

	switch (stage) {
	default: limit_sec = LIMIT_TIME; break; //~ 제한시간 설정 (include.h 에 명시되어있음)
	}
	string score;
	updateScore(plus_score_int);

	if (isForcedQuit) {
		isForcedQuit = false;
	}


	updateTimer(); //시간 관리

	

	last_sec = limit_sec - sec;
}

void gamePlay::Render() {
	SDL_RenderCopy(g_renderer, play_bg, NULL, NULL);


	///time bar
	SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255); //red
	SDL_RenderFillRect(g_renderer, &timebar_rect);


	renderCat();

	/// 스코프로 제한해둔 점수부분 렌더링
	{ //score text
		SDL_Rect tmp_r;
		tmp_r.x = g_window_margin + 80;
		tmp_r.y = 37;
		tmp_r.w = score_rect.w * 0.8;
		tmp_r.h = score_rect.h * 0.8;
		SDL_RenderCopy(g_renderer, score_text2, &score_rect, &tmp_r);
	}

	//! ************************** gameLogic **************************
	m_gameLogic.Render(); //~ 게임로직 렌더 함수 실행
	//! ************************** ********* **************************

	renderSetting();

	SDL_RenderPresent(g_renderer);
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
// 
//~ 게임 페이즈 변경
void gamePlay::changePhase(GamePhase status) {
	g_current_game_phase = status;
	Mix_HaltMusic();
	m_gameLogic.resetGame(); /// gameLogic 초기화

	switch (status) {
	case PHASE_ENDING_CLEAR:
		clear_reset = true;
		Mix_PlayMusic(clear_music, -1);
		break;
	case PHASE_ENDING_GAMEOVER:
		gameover_reset = true;
		Mix_PlayMusic(gameover_music, -1);
		break;
	case PHASE_MAIN:
		Mix_PlayMusic(main_music, -1);
		break;
	case PHASE_PLAYING:
		Mix_PlayMusic(play_music, -1);
		break;
	default:
		break;
	}
}



//~ 점수 업데이트
void gamePlay::updateScore(int s) {
	string front_score;
	update_score = to_string(s);

	//���� ���ڸ����� ����
	if (s == 0) {
		front_score = "000";
		update_score = front_score + update_score;
	}
	else if (s > 0 && s < 10) {
		front_score = "000";
		update_score = front_score + update_score;
	}
	else if (s >= 10 && s < 100) {
		front_score = "00";
		update_score = front_score + update_score;
	}
	else if (s >= 100 && s < 1000) {
		front_score = "0";
		update_score = front_score + update_score;
	}
	else { update_score = to_string(s); }


	TTF_Font* font = TTF_OpenFont("../../res/testRes/Galmuri14.ttf", 30);
	SDL_Color white = { 255,255,255,0 };
	SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(font, update_score.c_str(), white);
	//to_string(score).c_str()
	score_rect.x = 0;
	score_rect.y = 0;
	score_rect.w = tmp_surface->w;
	score_rect.h = tmp_surface->h;

	score_text2 = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);
	TTF_CloseFont(font);
}



void gamePlay::loadIMGs() {
	// BG
	SDL_Surface* bg_surface = IMG_Load("../../res/game page/game_background.png");
	play_bg = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);

	//cat
	SDL_Surface* cat_surface = IMG_Load("../../res/main page/main_cat1.png");
	cat = SDL_CreateTextureFromSurface(g_renderer, cat_surface);

	cat_rect.x = 0;
	cat_rect.y = 0;
	cat_rect.w = cat_surface->w;
	cat_rect.h = cat_surface->h;

	SDL_FreeSurface(cat_surface);

	SDL_Surface* cat_surface2 = IMG_Load("../../res/main page/main_cat2.png");
	cat2 = SDL_CreateTextureFromSurface(g_renderer, cat_surface2);

	cat_rect2.x = 0;
	cat_rect2.y = 0;
	cat_rect2.w = cat_surface2->w;
	cat_rect2.h = cat_surface2->h;

	SDL_FreeSurface(cat_surface2);


	{ //time bar
		timebar_rect.x = 0;
		timebar_rect.y = 290;
		timebar_rect.w = 540;
		timebar_rect.h = g_window_margin;
	}

	plus_score_int = org_score_int;

	{//setting img
		//mainscreen setting
		SDL_Surface* set_surface = IMG_Load("../../res/setting/setting_background.png");
		setting = SDL_CreateTextureFromSurface(g_renderer, set_surface);

		setting_rect.x = 0;
		setting_rect.y = 0;
		setting_rect.w = set_surface->w;
		setting_rect.h = set_surface->h;

		SDL_FreeSurface(set_surface);

	}
}


void gamePlay::loadSounds() {
	//music
	play_music = Mix_LoadMUS("../../res/testRes/testBGM3.mp3");
	if (play_music == 0) {
		cout << "Mix_LoadMUS(\"testBGM2.mp3\"): " << Mix_GetError() << endl;
	}

	setting_SoundEffect = Mix_LoadWAV("../../res/testRes/testSound.mp3");
}




void gamePlay::gotoHome() {
	Mix_PlayChannel(-1, setting_SoundEffect, 0);
						isSetting = false;
						isForcedQuit = true;
						string score;
						plus_score_int = org_score_int;
						score = original_score;
						ofstream ofs;
						ofs.open("../../res/testRes/scoreboard.txt");
						ofs << score;
						ofs.close();
						isChanged = true;
						SDL_Delay(33);

}

void gamePlay::updateTimer() {
	if (isChanged == false) {

		if (isSetting) {}
		else {


			count_ += 1;
			if (count_ != 0 && count_ % 33 == 0) {
				sec += 1; //play second
				timebar_rect.w = timebar_rect.w - 540 / limit_sec;

				if (last_sec == 0) { //~ 시간이 다 되었을 때
					SDL_Delay(1000);
					isChanged = true;
					//isForcedQuit = true;
					changePhase(PHASE_ENDING_GAMEOVER); //~ 게임오버로 페이즈 전환

				}

			}
		}
	}
	else if (isChanged) {
		timebar_rect.w = 540;

	}
}

void gamePlay::renderCat() {
	/// 고양이
	if (cat_status == false) {
		//cat image 1
		SDL_Rect tmp_r;
		tmp_r.x = 135;
		tmp_r.y = -110;
		tmp_r.w = cat_rect.w * 0.45;
		tmp_r.h = cat_rect.h * 0.45;
		SDL_RenderCopy(g_renderer, cat, &cat_rect, &tmp_r);

	}
	else {
		//cat image 2
		SDL_Rect tmp_r1;
		tmp_r1.x = 135;
		tmp_r1.y = -110;
		tmp_r1.w = cat_rect2.w * 0.45;
		tmp_r1.h = cat_rect2.h * 0.45;
		SDL_RenderCopy(g_renderer, cat2, &cat_rect2, &tmp_r1);
	}
}

void gamePlay::renderSetting() {
	/// 설정 버튼
	if (isSetting == true) {
		SDL_Rect tmp_r;
		tmp_r.x = 0;
		tmp_r.y = 0;
		tmp_r.w = setting_rect.w;
		tmp_r.h = setting_rect.h;
		SDL_RenderCopy(g_renderer, setting, &setting_rect, &tmp_r);

		if (isVolumeOff == false) {
			SDL_RenderCopy(g_renderer, volume_bt_off, &volume_rect, &volume_rect);
		}
		else if (isVolumeOff == true) {
			SDL_RenderCopy(g_renderer, volume_bt_on, &volume_rect, &volume_rect);
		}
	}
}

void gamePlay::MouseButtonEvents() {
	
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			int mouseX = event.button.x;
			int mouseY = event.button.y;


			//setting key
			if (isSetting == false && mouseX > setting_bt_rect.x && mouseY > setting_bt_rect.y &&
				mouseX < setting_bt_rect.x + setting_bt_rect.w && mouseY < setting_bt_rect.y + setting_bt_rect.h) {
				isSetting = !isSetting;
				Mix_PlayChannel(-1, setting_SoundEffect, 0);
			}

			if (isSetting == true) {
				if (mouseX > set_Xkey_rect.x && mouseY > set_Xkey_rect.y &&
					mouseX < set_Xkey_rect.x + set_Xkey_rect.w && mouseY < set_Xkey_rect.y + set_Xkey_rect.h) {
					isSetting = !isSetting;
					Mix_PlayChannel(-1, setting_SoundEffect, 0);
				}

				if (mouseX > volume_bt_rect.x && mouseY > volume_bt_rect.y &&
					mouseX < volume_bt_rect.x + volume_bt_rect.w && mouseY < volume_bt_rect.y + volume_bt_rect.h) {
					isVolumeOff = !isVolumeOff;
					Mix_PlayChannel(-1, setting_SoundEffect, 0);
					if (isVolumeOff == true) {
						Mix_PauseMusic();
					}
					else if (isVolumeOff == false) {
						Mix_ResumeMusic();
					}
				}

				//resume
				if (mouseX > resume_rect.x && mouseY > resume_rect.y &&
					mouseX < resume_rect.x + resume_rect.w && mouseY < resume_rect.y + resume_rect.h) {
					isSetting = !isSetting;
					Mix_PlayChannel(-1, setting_SoundEffect, 0);
				}

				//go to home
				if (mouseX > home_rect.x && mouseY > home_rect.y &&
					mouseX < home_rect.x + home_rect.w && mouseY < home_rect.y + home_rect.h) {
					gotoHome();

					changePhase(PHASE_MAIN); //~ 메인으로 페이즈 전환
				}
			}
		}
		else if (isSetting == false && event.button.button == SDL_BUTTON_RIGHT) {
			plus_score_int += 10;
		}
	}
}


//! ************************** gamePlay + gameLogic **************************

//~ 레벨 증가 로직
void gamePlay::increaseLevelLogic() {
	int prevLevel = m_gameLogic.getLevel();
	m_gameLogic.setLevel(++prevLevel);
}

//~ 맞춰야 할 블록 로드
void gamePlay::loadMahjongBlocks() {
	/// 기존 블록 제거 (stack영역 제거때문에 쓰는거임)
	m_gameLogic.ClearGame();

	/// 레벨에 따라 블록 로드
	string curLevel = "../../res/level/" + to_string(m_gameLogic.getLevel());
	srand(time(NULL));
	int seed = rand() % (m_gameLogic.countFiles(curLevel) / 3);					// 시드값 랜덤 생성 (랜덤 / (현재 레벨에 존재하는 파일 수  / 3))
	cout << "seed: " << seed << endl;
	m_gameLogic.LoadMahjongBlocksFromCSV(m_gameLogic.getLevel(), seed, 2);
}

//~ 맞춰야 할 블록 체크 및 로드
void gamePlay::checkAndLoadMahjongBlocks() {
	if (m_gameLogic.checkEmptyBlocks()) {
		if (m_gameLogic.getLevel() < m_gameLogic.getMaxLevel()) {
			increaseLevelLogic();
			loadMahjongBlocks();
		}
	}
}

//~ 게임 상태 체크
void gamePlay::checkGameStatus() {
	// g_stack이 더 이상 추가할 수 없는 경우 (예: 최대 스택 크기에 도달한 경우)
	if (m_gameLogic.getStack().size() >= MAX_STACK) {
		m_gameLogic.setStatus(STATUS_GAMEOVER);
		changePhase(PHASE_ENDING_GAMEOVER);
	}

	// MAX_LEVEL에 도달하고 해당 레벨의 블록이 모두 비어있는 경우
	if (m_gameLogic.getLevel() == m_gameLogic.getMaxLevel() && m_gameLogic.checkEmptyBlocks()) {
		m_gameLogic.setStatus(STATUS_GAMECLEAR);
		changePhase(PHASE_ENDING_CLEAR);
	}
}