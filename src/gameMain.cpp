#include "gameMain.h"






gameMain::gameMain() {
	isBasicCat = true;
	isRule = false;
	isSetting = false;
	isVolumeOff = false;
	isDifficulty = false;

	loadImgs();
	loadSounds();
	loadTxts();
}

gameMain::~gameMain() {
	Mix_FreeMusic(main_music);
	Mix_FreeChunk(SoundEffect);
	SDL_DestroyTexture(inter_bt);
	SDL_DestroyTexture(main_bg);
	SDL_DestroyTexture(difficulty_bg);
	SDL_DestroyTexture(cat);
	SDL_DestroyTexture(setting);
	SDL_DestroyTexture(cat_sit);
	SDL_DestroyTexture(cat_sleep);
	SDL_DestroyTexture(score_txt);
	SDL_DestroyTexture(exp_txt);

	TTF_CloseFont(score_font);

	SDL_Quit();
	TTF_Quit();
}

void gameMain::HandleEvents() {
	//SDL_Event event;
	if (SDL_PollEvent(&event)) {

		switch (event.type) {

		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_SPACE) {
				changePhaseToPlay();
			}

			if (isDifficulty) {
				//난이도 조절(숫자 키 1~4)
				if (event.key.keysym.sym == SDLK_1) {
					m_gameplay.setLastSec(120);
					m_gameplay.setAddSec(1);
					m_gameplay.setAddScore(5);
					std::cout << m_gameplay.getLastSec() << " "<< m_gameplay.getAddSec() << " " << m_gameplay.getAddScore() << std::endl;
					changePhaseToPlay();
				}
				else if (event.key.keysym.sym == SDLK_2) {
					m_gameplay.setLastSec(90);
					m_gameplay.setAddSec(1);
					m_gameplay.setAddScore(10);
					std::cout << m_gameplay.getLastSec() << " " << m_gameplay.getAddSec() << " " << m_gameplay.getAddScore() << std::endl;
					changePhaseToPlay();
				}
				else if (event.key.keysym.sym == SDLK_3) {
					m_gameplay.setLastSec(45);
					m_gameplay.setAddSec(2);
					m_gameplay.setAddScore(20);
					std::cout << m_gameplay.getLastSec() << " " << m_gameplay.getAddSec() << " " << m_gameplay.getAddScore() << std::endl;
					changePhaseToPlay();
				}
				else if (event.key.keysym.sym == SDLK_4) {
					m_gameplay.setLastSec(10);
					m_gameplay.setAddSec(3);
					m_gameplay.setAddScore(30);
					std::cout << m_gameplay.getLastSec() << " " << m_gameplay.getAddSec() << " " << m_gameplay.getAddScore() << std::endl;
					changePhaseToPlay();
				}
			
			
			}break;

		default: break;
		}

		MouseButtonEvents();

		

	}
}

void gameMain::Update() {
	count_ = 0;
	sec = 0;
	last_sec = 0;
	isChanged  = false;

	timebar_rect.w = 540;

}

void gameMain::Render() {
	SDL_RenderCopy(g_renderer, main_bg, NULL, NULL); 
	SDL_RenderCopy(g_renderer, inter_bt, NULL, NULL);
	
	renderScore();
	renderCat();
	renderButtons();

	SDL_RenderPresent(g_renderer);
}



///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////



void gameMain::changePhaseToPlay() { 
	g_current_game_phase = PHASE_PLAYING;
	Mix_HaltMusic();
	Mix_PlayMusic(play_music, -1);
	isDifficulty = false;
}


void gameMain::loadImgs() {
	// BG
	SDL_Surface* bg_surface = IMG_Load("../../res/main page/main_background_2.png");
	main_bg = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);


	//playbutton area
	{
		playBT_dir_rect.x = 140;
		playBT_dir_rect.y = 850;
		playBT_dir_rect.w = 260;
		playBT_dir_rect.h = 90;
	}


	//question mark area
	{
		ques_rect.x = 10;
		ques_rect.y = 870;
		ques_rect.w = 50;
		ques_rect.h = 50;
	}

	{//rule X key area
		rule_Xkey_rect.x = 430;
		rule_Xkey_rect.y = 120;
		rule_Xkey_rect.w = 30;
		rule_Xkey_rect.h = 30;
	}


	{
		//rule image
		SDL_Surface* rule_surface = IMG_Load("../../res/main page/rule.png");
		rule = SDL_CreateTextureFromSurface(g_renderer, rule_surface);

		rule_rect.x = 0;
		rule_rect.y = 0;
		rule_rect.w = rule_surface->w;
		rule_rect.h = rule_surface->h;

		SDL_FreeSurface(rule_surface);
	}


	{//난이도 조절하는 단계 이미지
		SDL_Surface* surface = IMG_Load("../../res/testRes/testPlayBG.png");
		difficulty_bg = SDL_CreateTextureFromSurface(g_renderer, surface);
		SDL_FreeSurface(surface);
	}


	//cat
	{
		SDL_Surface* cat_surface = IMG_Load("../../res/main page/main_sprite.png");
		cat = SDL_CreateTextureFromSurface(g_renderer, cat_surface);

		cat_rect.x = 0;
		cat_rect.y = 0;
		cat_rect.w = cat_surface->w;
		cat_rect.h = cat_surface->h;

		SDL_FreeSurface(cat_surface);
	}

	{ //cat playing
		SDL_Surface* c_surface = IMG_Load("../../res/main page/play_sprite.png");
		cat_sit = SDL_CreateTextureFromSurface(g_renderer, c_surface);

		cat_main_rect.x = 0;
		cat_main_rect.y = 0;
		cat_main_rect.w = c_surface->w;
		cat_main_rect.h = c_surface->h;

		SDL_FreeSurface(c_surface);

	}

	{ //cat sleeping
		SDL_Surface* c_surface = IMG_Load("../../res/main page/sleep_sprite.png");
		cat_sleep = SDL_CreateTextureFromSurface(g_renderer, c_surface);
		SDL_FreeSurface(c_surface);
	}





	{//setting img
		//mainscreen setting
		SDL_Surface* set_surface = IMG_Load("../../res/setting/main_setting_background.png");
		setting = SDL_CreateTextureFromSurface(g_renderer, set_surface);

		setting_rect.x = 0;
		setting_rect.y = 0;
		setting_rect.w = set_surface->w;
		setting_rect.h = set_surface->h;

		SDL_FreeSurface(set_surface);

	}

	{//interaction button img
		//mainscreen setting
		SDL_Surface* i_surface = IMG_Load("../../res/main page/main_interaction_btn.png");
		inter_bt = SDL_CreateTextureFromSurface(g_renderer, i_surface);
		SDL_FreeSurface(i_surface);
	}

	{
		sleepBT_rect.x = 20;
		sleepBT_rect.y = 200;
		sleepBT_rect.w = 80;
		sleepBT_rect.h = 80;
	}
	
	{
		sitBT_rect.x = 20;
		sitBT_rect.y = 110;
		sitBT_rect.w = 80;
		sitBT_rect.h = 80;
	}

	
}


void gameMain::loadSounds() {
	//music
	main_music = Mix_LoadMUS("../../res/testRes/testBGM2.mp3");
	if (main_music == 0) {
		cout << "Mix_LoadMUS(\"testBGM2.mp3\"): " << Mix_GetError() << endl;
	}



	//sound effect(play button)
	SoundEffect = Mix_LoadWAV("../../res/testRes/testSound.mp3");
}

void gameMain::loadTxts() {
	{ //score text

		ifstream file("../../res/testRes/scoreboard.txt"); //read scoreboard
		getline(file, original_score);
		org_score_int = stoi(original_score);
		file.close();
		string front_score;
		string new_score;

		//점수 네자리수로 고정
		if (org_score_int == 0) {
			front_score = "0000";
			new_score = front_score + original_score;
		}
		else if (org_score_int > 0 && org_score_int < 10) {
			front_score = "0000";
			new_score = front_score + original_score;
		}
		else if (org_score_int >= 10 && org_score_int < 100) {
			front_score = "000";
			new_score = front_score + original_score;
		}
		else if (org_score_int >= 100 && org_score_int < 1000) {
			front_score = "00";
			new_score = front_score + original_score;
		}
		else if (org_score_int >= 1000 && org_score_int < 10000) {
			front_score = "0";
			new_score = front_score + original_score;
		}
		else { new_score = original_score; }

		score_font = TTF_OpenFont("../../res/testRes/Galmuri14.ttf", 20);
		SDL_Color white = { 255,255,255,0 };
		SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(score_font, new_score.c_str(), white);
		//to_string(score).c_str()
		score_rect.x = 0;
		score_rect.y = 0;
		score_rect.w = tmp_surface->w;
		score_rect.h = tmp_surface->h;

		score_txt = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
		SDL_FreeSurface(tmp_surface);
		

	}
}

void gameMain::renderScore() {
	{ //score text

		SDL_DestroyTexture(score_txt); //메모리 누수 관리


		ifstream file("../../res/testRes/scoreboard.txt"); //read scoreboard
		getline(file, original_score);
		org_score_int = stoi(original_score);
		file.close();
		string front_score;
		string new_score;

		//점수 네자리수로 고정
		if (org_score_int == 0) {
			front_score = "0000";
			new_score = front_score + original_score;
		}
		else if (org_score_int > 0 && org_score_int < 10) {
			front_score = "0000";
			new_score = front_score + original_score;
		}
		else if (org_score_int >= 10 && org_score_int < 100) {
			front_score = "000";
			new_score = front_score + original_score;
		}
		else if (org_score_int >= 100 && org_score_int < 1000) {
			front_score = "00";
			new_score = front_score + original_score;
		}
		else if (org_score_int >= 1000 && org_score_int < 10000) {
			front_score = "0";
			new_score = front_score + original_score;
		}
		else { new_score = original_score; }

		
		SDL_Color white = { 255,255,255,0 };
		SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(score_font, new_score.c_str(), white);
		//to_string(score).c_str()
		score_rect.x = 0;
		score_rect.y = 0;
		score_rect.w = tmp_surface->w;
		score_rect.h = tmp_surface->h;

		score_txt = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
		SDL_FreeSurface(tmp_surface);

	}
	{ //score text
		SDL_Rect tmp_r;
		tmp_r.x = g_window_margin + 77;
		tmp_r.y = 38;
		tmp_r.w = score_rect.w;
		tmp_r.h = score_rect.h;
		SDL_RenderCopy(g_renderer, score_txt, &score_rect, &tmp_r);
	}
}

void gameMain::MouseButtonEvents() {
	
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			int mouseX = event.button.x;
			int mouseY = event.button.y;


			if (isRule == false && isSetting == false && isDifficulty == false) { //main screen

				//click play button
				if (mouseX > playBT_dir_rect.x && mouseY > playBT_dir_rect.y &&
					mouseX < playBT_dir_rect.x + playBT_dir_rect.w && mouseY < playBT_dir_rect.y + playBT_dir_rect.h) {
					isDifficulty = !isDifficulty;
					Mix_PlayChannel(-1, SoundEffect, 0);
				}

				//click question mark -> open rule page
				if (mouseX > ques_rect.x && mouseY > ques_rect.y &&
					mouseX < ques_rect.x + ques_rect.w && mouseY < ques_rect.y + ques_rect.h) {
					isRule = !isRule;
					Mix_PlayChannel(-1, SoundEffect, 0);

				}

				//setting key
				if (mouseX > setting_bt_rect.x && mouseY > setting_bt_rect.y &&
					mouseX < setting_bt_rect.x + setting_bt_rect.w && mouseY < setting_bt_rect.y + setting_bt_rect.h) {
					isSetting = !isSetting;
					Mix_PlayChannel(-1, SoundEffect, 0);
				}

				//sleep key
				if (mouseX > sleepBT_rect.x && mouseY > sleepBT_rect.y &&
					mouseX < sleepBT_rect.x + sleepBT_rect.w && mouseY < sleepBT_rect.y + sleepBT_rect.h) {
					isBasicCat = false;
					m_RandCat = 2;
					Mix_PlayChannel(-1, SoundEffect, 0);
				}

				//sit key
				if (mouseX > sitBT_rect.x && mouseY > sitBT_rect.y &&
					mouseX < sitBT_rect.x + sitBT_rect.w && mouseY < sitBT_rect.y + sitBT_rect.h) {
					isBasicCat = false;
					m_RandCat = 3;
					Mix_PlayChannel(-1, SoundEffect, 0);
				}
			}


			//click X -> back to main screen
			if (isRule == true && mouseX > rule_Xkey_rect.x && mouseY > rule_Xkey_rect.y &&
				mouseX < rule_Xkey_rect.x + rule_Xkey_rect.w && mouseY < rule_Xkey_rect.y + rule_Xkey_rect.h) {
				isRule = !isRule;
				Mix_PlayChannel(-1, SoundEffect, 0);
			}

			if (isSetting == true) {
				//click X key
				if (mouseX > set_Xkey_rect.x && mouseY > set_Xkey_rect.y &&
					mouseX < set_Xkey_rect.x + set_Xkey_rect.w && mouseY < set_Xkey_rect.y + set_Xkey_rect.h) {
					isSetting = !isSetting;
					Mix_PlayChannel(-1, SoundEffect, 0);
				}

				//volume on -> off
				if (mouseX > volume_bt_rect.x && mouseY > volume_bt_rect.y &&
					mouseX < volume_bt_rect.x + volume_bt_rect.w && mouseY < volume_bt_rect.y + volume_bt_rect.h) {
					isVolumeOff = !isVolumeOff;
					Mix_PlayChannel(-1, SoundEffect, 0);
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
					Mix_PlayChannel(-1, SoundEffect, 0);
				}

				//exit game
				if (mouseX > home_rect.x && mouseY > home_rect.y &&
					mouseX < home_rect.x + home_rect.w && mouseY < home_rect.y + home_rect.h) {
					exit(1);
				}
			}



		}
	}
}

void gameMain::renderButtons() {
	if (isRule) {
		SDL_Rect tmp_r;
		tmp_r.x = 0;
		tmp_r.y = 0;
		tmp_r.w = rule_rect.w;
		tmp_r.h = rule_rect.h;
		SDL_RenderCopy(g_renderer, rule, &rule_rect, &tmp_r);
	}

	if (isSetting) {
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

	if (isDifficulty) {
		SDL_RenderCopy(g_renderer, difficulty_bg, NULL, NULL);
	}
}

void gameMain::renderCat() {
	if (isBasicCat) {
		if (cat_status == false) {
			//cat image 1
			SDL_Rect tmp_r;
			tmp_r.x = 0;
			tmp_r.y = 0;
			tmp_r.w = cat_rect.w;
			tmp_r.h = cat_rect.h;
			SDL_RenderCopy(g_renderer, cat, &cat_rect, &tmp_r);

		}
		else {
			//cat image 2
			SDL_Rect tmp_r1;
			tmp_r1.x = -540;
			tmp_r1.y = 0;
			tmp_r1.w = cat_rect.w;
			tmp_r1.h = cat_rect.h;
			SDL_RenderCopy(g_renderer, cat, &cat_rect, &tmp_r1);
		}
	}
	else {
		SDL_Rect tmp_r1;
		tmp_r1.y = 0;
		tmp_r1.w = cat_main_rect.w;
		tmp_r1.h = cat_main_rect.h;

		switch (sprite_num) {
		case 0:	tmp_r1.x = 0; break;
		case 1: tmp_r1.x = -WINDOW_WIDTH; break;
		case 2: tmp_r1.x = -WINDOW_WIDTH * 2; break;
		case 3:  tmp_r1.x = -WINDOW_WIDTH * 3; break;
		default: 0; break;
		}

		switch (m_RandCat) {
		case 2: SDL_RenderCopy(g_renderer, cat_sit, &cat_main_rect, &tmp_r1); break;
		case 3: SDL_RenderCopy(g_renderer, cat_sleep, &cat_main_rect, &tmp_r1); break;
		default: SDL_RenderCopy(g_renderer, cat_sit, &cat_main_rect, &tmp_r1); break;
		}
	}
}

