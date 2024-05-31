#include "gamePlay.h"

extern SDL_Texture* exp_text; //"EXP" text

//! ******************** 생성자 소멸자 ******************** 

gamePlay::gamePlay() {

	srand(time(NULL));
	RandI = rand() % 6 + 5; //5~10초 중 하나 랜덤 반환// 다른 상태 중일 때는 시간이 흐르지 않도록..
	p_RandCat = rand() % 4; //cat status 설정
	isBasicCat = true;
	isForcedQuit = false;
	limit_sec = LIMIT_TIME;
	isChanged = false;
	sec = 0;
	count_ = 0;
	stage = 1;
	sprite_num = 0;

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
	SDL_DestroyTexture(cat_sit);
	SDL_DestroyTexture(cat_sleep);
	SDL_DestroyTexture(cat_walk_left);
	SDL_DestroyTexture(cat_walk_right);
	m_gameLogic.ClearGame();
	SDL_Quit();
	TTF_Quit();
}


//! ********************** 기본 함수 **********************

void gamePlay::HandleEvents() {

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
				writeScore(update_score);			//~ 점수판에 점수 기록
				changePhase(PHASE_ENDING_CLEAR);	//~ 클리어로 페이즈 전환
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

		m_gameLogic.HandleEvents(event); //~ 게임로직 이벤트 처리 함수 실행
		MouseButtonEvents();
	}
	
}

void gamePlay::Update() {
	//m_gameLogic.LoadMahjongBlocksIfEmpty(m_gameLogic.getLevel());
	m_gameLogic.RemoveSameTypeBlocks();
	if (m_gameLogic.getIsPop()) {
		addSeconds(ADD_TIME);
		m_gameLogic.setIsPop(false); //~ RemoveSameTypeBlocks()에서 true가 된 isPop 을 다시 fasle 처리
	}
	m_gameLogic.printStatusChange();		//~ 게임 상태 출력
	checkAndLoadMahjongBlocks();			//~ 맞춰야 할 블록 체크 및 로드
	stageLimitTime();						//~ 제한시간 설정
	updateScore(m_gameLogic.getScore());	//~ 점수 업데이트
	checkQuit();							//~ 강제종료 체크
	updateTimer();							//~ 시간 및 타이머 업데이트
	checkGameStatus();						//~ 게임 상태 체크
	updateCatStatus();						//~ 고양이 상태 변경
	changeCatAnimation();					//~ g_curType에 따른 고양이 애니메이션 변경
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

void gamePlay::resetGame() {
	m_gameLogic.resetGame();
	resetTimer();
	isChanged = false;
	isForcedQuit = false;
}

//! ********************** 페이즈 전환 **********************
//~ 게임 페이즈 변경
void gamePlay::changePhase(GamePhase status) {
	g_current_game_phase = status;
	Mix_HaltMusic();
	m_gameLogic.resetGame();	// gameLogic 초기화
	switch (status) {
	case PHASE_ENDING_CLEAR:
		clear_reset = true;
		Mix_PlayMusic(clear_music, -1);
		writeScore(update_score); //~ 점수판에 점수 기록
		resetGame();
		break;
	case PHASE_ENDING_GAMEOVER:
		gameover_reset = true;
		Mix_PlayMusic(gameover_music, -1);
		writeScore(update_score); //~ 점수판에 점수 기록
		resetGame();
		break;
	case PHASE_MAIN:
		Mix_PlayMusic(main_music, -1);
		resetGame();
		break;
	case PHASE_PLAYING:
		//resetTimer(); // 타이머 초기화
		Mix_PlayMusic(play_music, -1);
		resetGame();
		break;
	default:
		break;
	}
}

//~ 게임 홈으로 이동
void gamePlay::gotoHome() {
	Mix_PlayChannel(-1, setting_SoundEffect, 0);
	isSetting = false;
	isForcedQuit = true;
	
	writeScore(original_score); //~ 점수판에 점수 기록

	isChanged = true;
	SDL_Delay(33);
	resetGame(); // 게임 초기화
}


//! //! ********************** 타이머 관련 **********************
//~ 타이머 리셋
void gamePlay::resetTimer() {
	sec = 0;
	last_sec = limit_sec;
	changeTimebar();
}

//~ 제한시간 설정 (LIMIT_TIME은 include.h 에 명시되어있음)
void gamePlay::stageLimitTime() {
	switch (stage) {
	default: limit_sec = LIMIT_TIME; break;
	}
}

//~ 타이머 업데이트
void gamePlay::updateTimer() {
	if (!isChanged && !isSetting) {
		count_ += 1;
		if (count_ % 33 == 0) {
			sec += 1;
			last_sec = limit_sec - sec;
			changeTimebar();

			if (last_sec == 0) {
				SDL_Delay(500);
				isChanged = true;
				resetGame();
				changePhase(PHASE_ENDING_GAMEOVER);
			}
		}
	}
}

//~ 실시간 타임바 렌더링
void gamePlay::changeTimebar() {
	timebar_rect.w = 540 * (static_cast<double>(last_sec) / limit_sec);
}


//! ********************** 점수 관련 **********************
//~ 점수 업데이트
void gamePlay::updateScore(int s) {
	string front_score;
	int updateScore_int = s + org_score_int;
	update_score = to_string(updateScore_int);

	// 점수 4자리수로 맞추기
	if (updateScore_int == 0) {
		front_score = "000";
		update_score = front_score + update_score;
	}
	else if (updateScore_int > 0 && updateScore_int < 10) {
		front_score = "000";
		update_score = front_score + update_score;
	}
	else if (updateScore_int >= 10 && updateScore_int < 100) {
		front_score = "00";
		update_score = front_score + update_score;
	}
	else if (updateScore_int >= 100 && updateScore_int < 1000) {
		front_score = "0";
		update_score = front_score + update_score;
	}
	else { update_score = to_string(updateScore_int); }


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

//~ gameLogic의 isPop이 true일 경우, 시간 추가
void gamePlay::addSeconds(int seconds) {
	sec = max(0, sec - seconds);
	last_sec = limit_sec - sec;
	changeTimebar();
}
/// 아래함수 미구현
//~ 추가점수 구현 (기본 점수 100점, 1초당 25점 감소)
void gamePlay::updateAddScore() {
	// 미구현
}

//~ 점수판에 점수 기록
void gamePlay::writeScore(const string& s) {
	ofstream ofs("../../res/testRes/scoreboard.txt");
	if (ofs.is_open()) {
		ofs << s;
		ofs.close();
	}
}


//! ********************** 고양이 관련 **********************
//~ 고양이 상태 업데이트
void gamePlay::updateCatStatus() {
	if (isBasicCat && sec == RandI) {
		isBasicCat = false;
	}
}

//~ g_curType 에 따른 고양이 애니메이션 출력
void gamePlay::changeCatAnimation() {
	static int prevType = NONE;
	if (g_curType != prevType) {
		switch (g_curType) {
		case MahjongType_0: 
			cout << "A" << endl; // 물 블록
			// 여기에 '목마름' 애니메이션 실행시켜주세요
			break;
		case MahjongType_1: 
			cout << "B" << endl; // 캣타워 블록
			// 여기에 '잠자기' 애니메이션 실행시켜주세요
			break;
		case MahjongType_2: 
			cout << "C" << endl; // 생선 블록
			// 여기에 '배부름' 애니메이션 실행시켜주세요
			break;
		case MahjongType_3: 
			cout << "D" << endl; // 쥐 블록
			// 여기에 '놀아주기' 애니메이션 실행시켜주세요
			break;
		case NONE: break;
		default: break;
		}
		prevType = g_curType;
	}

	g_curType = NONE;
}


//! ********************** 데이터 import 및 Rendering **********************
//~ 이미지 로드
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

	{ //cat sitting
		SDL_Surface* c_surface = IMG_Load("../../res/game page/cat_sit_sprite.png");
		cat_sit = SDL_CreateTextureFromSurface(g_renderer, c_surface);

		cat_play_rect.x = 0;
		cat_play_rect.y = 0;
		cat_play_rect.w = c_surface->w;
		cat_play_rect.h = c_surface->h;

		SDL_FreeSurface(c_surface);

	}

	{ //cat sleeping
		SDL_Surface* c_surface = IMG_Load("../../res/game page/cat_sleep_sprite.png");
		cat_sleep = SDL_CreateTextureFromSurface(g_renderer, c_surface);
		SDL_FreeSurface(c_surface);
	}

	{//cat walk left
		SDL_Surface* c_surface = IMG_Load("../../res/game page/cat_walk_left_sprite.png");
		cat_walk_left = SDL_CreateTextureFromSurface(g_renderer, c_surface);
		SDL_FreeSurface(c_surface);
	}

	{//cat walk right
		SDL_Surface* c_surface = IMG_Load("../../res/game page/cat_walk_right_sprite.png");
		cat_walk_right = SDL_CreateTextureFromSurface(g_renderer, c_surface);
		SDL_FreeSurface(c_surface);
	}
}

//~ 사운드 로드
void gamePlay::loadSounds() {
	//music
	play_music = Mix_LoadMUS("../../res/testRes/testBGM3.mp3");
	if (play_music == 0) {
		cout << "Mix_LoadMUS(\"testBGM2.mp3\"): " << Mix_GetError() << endl;
	}

	setting_SoundEffect = Mix_LoadWAV("../../res/testRes/testSound.mp3");
}

//~ 고양이 렌더링
void gamePlay::renderCat() {
	/// 고양이
	if (isBasicCat) {
		if (cat_status == false) {
			//cat image 1
			SDL_Rect tmp_r;
			tmp_r.x = 180;
			tmp_r.y = 0;
			tmp_r.w = cat_rect.w * 0.3;
			tmp_r.h = cat_rect.h * 0.3;
			SDL_RenderCopy(g_renderer, cat, &cat_rect, &tmp_r);


		}
		else {
			//cat image 2
			SDL_Rect tmp_r1;
			tmp_r1.x = 180;
			tmp_r1.y = 0;
			tmp_r1.w = cat_rect2.w * 0.3;
			tmp_r1.h = cat_rect2.h * 0.3;
			SDL_RenderCopy(g_renderer, cat2, &cat_rect2, &tmp_r1);
		}
	}
	else {
		SDL_Rect tmp_r1;
		tmp_r1.y = 0;
		tmp_r1.w = cat_play_rect.w;
		tmp_r1.h = cat_play_rect.h;

		switch (sprite_num) {
		case 0:	tmp_r1.x = 0; break;
		case 1: if (p_RandCat == 0 || p_RandCat == 1) { tmp_r1.x = -WINDOW_WIDTH; break; }
			  else if (p_RandCat == 2) { tmp_r1.x = -WINDOW_WIDTH - 20; break; }
			  else { tmp_r1.x = -WINDOW_WIDTH + 20; break; }

		case 2: if (p_RandCat == 0 || p_RandCat == 1) { tmp_r1.x = -WINDOW_WIDTH * 2; break; }
			  else if (p_RandCat == 2) { tmp_r1.x = -WINDOW_WIDTH * 2 - 40; break; }
			  else { tmp_r1.x = -WINDOW_WIDTH * 2 + 40; break; }

		case 3: if (p_RandCat == 0 || p_RandCat == 1) { tmp_r1.x = -WINDOW_WIDTH * 3; break; }
			  else if (p_RandCat == 2) { tmp_r1.x = -WINDOW_WIDTH * 3 - 60; break; }
			  else { tmp_r1.x = -WINDOW_WIDTH * 3 + 60; break; }

		default: 0; break;
		}

		switch (p_RandCat) {
		case 0: SDL_RenderCopy(g_renderer, cat_sit, &cat_play_rect, &tmp_r1); break;

		case 1: SDL_RenderCopy(g_renderer, cat_sleep, &cat_play_rect, &tmp_r1); break;

		case 2: SDL_RenderCopy(g_renderer, cat_walk_left, &cat_play_rect, &tmp_r1); break;

		case 3: SDL_RenderCopy(g_renderer, cat_walk_right, &cat_play_rect, &tmp_r1); break;


		default: SDL_RenderCopy(g_renderer, cat_sit, &cat_play_rect, &tmp_r1); break;
		}
	}
}

//~ 설정창 렌더링
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


//! ********************** 마우스 이벤트 **********************
//~ 마우스 버튼 이벤트 처리
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

//~ 강제종료 체크
void gamePlay::checkQuit() {
	if (isForcedQuit) {
		isForcedQuit = false;
	}
}

