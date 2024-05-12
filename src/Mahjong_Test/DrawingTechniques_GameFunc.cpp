#include "DrawingTechniques_GameFunc.h"
#include "obj_Mahjong_Derived.h"

// sprite의 위치를 나타내는 열거형
enum Position {
	BOTTOM_LEFT = 0,
	BOTTOM_CENTER = 1,
	BOTTOM_RIGHT = 2,

	CENTER_LEFT = 3,
	CENTER_CENTER = 4,
	CENTER_RIGHT = 5,

	TOP_LEFT = 6,
	TOP_CENTER = 7,
	TOP_RIGHT = 8
};

std::vector<Mahjong_A> g_fires;

//~ 오디오
Mix_Chunk* wave1_;
Mix_Music* music1_;

//~ 폰트
SDL_Texture* text_score_;
SDL_Rect text_score_rect_;
TTF_Font* game_font_;

//~ 배경 이미지
SDL_Rect g_bg_source_rect;
SDL_Rect g_bg_destination_rect;
SDL_Texture* g_bg_texture;

//~ 플레이어 스프라이트 이미지
SDL_Texture* g_player_sheet_texture;
SDL_Rect g_player_source_rects[9];
SDL_Rect g_player_destination_rect;


int g_player_sprite_num;
int g_current_player_id;

int g_score; // 점수

int g_input; // 사용자의 키를 입력받는 변수
int g_maxFireCount = MAGAZINE_SIZE; // 총알 최대 발사 수

void InitGame() {
	g_flag_running = true;
	game_font_ = TTF_OpenFont("../Resources/neodgm.ttf", 48);

	g_input = 0;
	g_score = 0;

	//~ BG
	// 배경화면은 3가지 중 랜덤으로 선택됩니다
	srand(static_cast<unsigned int>(time(NULL))); // 랜덤 시드 초기화
	int randomNumber = rand() % 3; // 0, 1, 2 중 랜덤한 숫자 생성
	string bgFilename = "../Resources/BG0" + to_string(randomNumber) + ".jpg"; // 파일 이름 생성
	SDL_Surface* bg_surface = IMG_Load(bgFilename.c_str());
	g_bg_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);

	g_bg_source_rect.x = 0;
	g_bg_source_rect.y = 0;
	g_bg_source_rect.w = WINDOW_WIDTH;
	g_bg_source_rect.h = WINDOW_HEIGHT;

	g_bg_destination_rect.x = 0;
	g_bg_destination_rect.y = 0;
	g_bg_destination_rect.w = WINDOW_WIDTH;
	g_bg_destination_rect.h = WINDOW_HEIGHT;

	

	// Player Character
	g_player_sprite_num = 3;
	g_current_player_id = 0;

	SDL_Surface* player_sheet_surface = IMG_Load("../Resources/plane_9_direction.png");
	
	g_player_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, player_sheet_surface);
	SDL_FreeSurface(player_sheet_surface);
	// 중괄호로 묶은건 구조체에서 초기화한 변수에 차례대로 들어감

	g_player_source_rects[TOP_LEFT] = { 0  , 0, 200, 200 };
	g_player_source_rects[TOP_CENTER] = { 200, 0, 200, 200 };
	g_player_source_rects[TOP_RIGHT] = { 400, 0, 200, 200 };
	g_player_source_rects[CENTER_LEFT] = { 0  , 200, 200, 200 };
	g_player_source_rects[CENTER_CENTER] = { 200, 200, 200, 200 };
	g_player_source_rects[CENTER_RIGHT] = { 400, 200, 200, 200 };
	g_player_source_rects[BOTTOM_LEFT] = { 0  , 400, 200, 200 };
	g_player_source_rects[BOTTOM_CENTER] = { 200, 400, 200, 200 };
	g_player_source_rects[BOTTOM_RIGHT] = { 400, 400, 200, 200 };


	// 그려질 위치
	g_player_destination_rect.x = WINDOW_WIDTH /2 - 100;
	g_player_destination_rect.y = WINDOW_HEIGHT/2;
	g_player_destination_rect.w = g_player_source_rects[0].w; // 이미지의 너비
	g_player_destination_rect.h = g_player_source_rects[0].h;

	// 음악 설정
	Set2Music();
}

void HandleEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
			switch (event.key.keysym.sym) {
			case SDLK_UP:
				if (event.type == SDL_KEYDOWN) {
					g_input |= 1; // 상단 비트 설정
				}
				else {
					g_input &= ~1; // 상단 비트 해제
				}
				break;
			case SDLK_DOWN:
				if (event.type == SDL_KEYDOWN) {
					g_input |= 2; // 하단 비트 설정
				}
				else {
					g_input &= ~2; // 하단 비트 해제
				}
				break;
			case SDLK_LEFT:
				if (event.type == SDL_KEYDOWN) {
					g_input |= 4; // 좌측 비트 설정
					g_input &= ~8; // 우측 비트 해제
				}
				else {
					g_input &= ~4; // 좌측 비트 해제
					if (g_input & 8) { // 우측 비트가 설정되어 있으면
						g_input |= 8; // 우측 비트 설정 (좌측 해제 후 우측 유지)
					}
				}
				break;
			case SDLK_RIGHT:
				if (event.type == SDL_KEYDOWN) {
					g_input |= 8; // 우측 비트 설정
					g_input &= ~4; // 좌측 비트 해제
				}
				else {
					g_input &= ~8; // 우측 비트 해제
					if (g_input & 4) { // 좌측 비트가 설정되어 있으면
						g_input |= 4; // 좌측 비트 설정 (우측 해제 후 좌측 유지)
					}
				}
				break;
			case SDLK_SPACE:
				if (event.type == SDL_KEYDOWN) {
					if (g_fires.size() < g_maxFireCount) {
						g_fires.emplace_back(g_player_destination_rect.x + g_player_destination_rect.w / 2, g_player_destination_rect.y, g_renderer);
						cout << "MAGAZINE : " << g_fires.size() << "/" << MAGAZINE_SIZE << endl;
						g_score += 10; // 점수 10점 증가
					}
					else {
						cout << "Maximum fire count reached!" << endl;
					}
				}
				break;
			}
		}
	}
}
void Update() {

	//! 캐릭터 이동
	const float speed = MOVE_SPEED; // 이동 속도
	float dx = 0, dy = 0;

	if (g_input & 1) dy -= speed; // 상단 이동
	if (g_input & 2) dy += speed; // 하단 이동
	if (g_input & 4) dx -= speed; // 좌측 이동
	if (g_input & 8) dx += speed; // 우측 이동

	//~ 대각선 이동 속도 조절
	if (dx != 0 && dy != 0) {
		dx *= 0.7071f; // sqrt(2)/2
		dy *= 0.7071f;
	}

	//~ 캐릭터 이미지 변경
	if ((g_input & 4) && (g_input & 1)) {
		g_current_player_id = TOP_LEFT; // 좌상단 이동
	}
	else if ((g_input & 4) && (g_input & 2)) {
		g_current_player_id = BOTTOM_LEFT; // 좌하단 이동
	}
	else if ((g_input & 8) && (g_input & 1)) {
		g_current_player_id = TOP_RIGHT; // 우상단 이동
	}
	else if ((g_input & 8) && (g_input & 2)) {
		g_current_player_id = BOTTOM_RIGHT; // 우하단 이동
	}
	else if (g_input & 4) {
		g_current_player_id = CENTER_LEFT; // 좌측 이동
	}
	else if (g_input & 8) {
		g_current_player_id = CENTER_RIGHT; // 우측 이동
	}
	else if (g_input & 1) {
		g_current_player_id = TOP_CENTER; // 상단 이동
	}
	else if (g_input & 2) {
		g_current_player_id = BOTTOM_CENTER; // 하단 이동
	}
	else {
		g_current_player_id = CENTER_CENTER; // 기본 이미지
	}

	g_player_destination_rect.x += static_cast<int>(dx);
	g_player_destination_rect.y += static_cast<int>(dy);

	//! 캐릭터 위치 경계 처리
	if (g_player_destination_rect.x < 0) {
		g_player_destination_rect.x = 0;
	}
	if (g_player_destination_rect.x > WINDOW_WIDTH - g_player_destination_rect.w) {
		g_player_destination_rect.x = WINDOW_WIDTH - g_player_destination_rect.w;
	}
	if (g_player_destination_rect.y < 0) {
		g_player_destination_rect.y = 0;
	}
	if (g_player_destination_rect.y > WINDOW_HEIGHT - g_player_destination_rect.h) {
		g_player_destination_rect.y = WINDOW_HEIGHT - g_player_destination_rect.h;
	}

	//! 총알 이동
	for (auto it = g_fires.begin(); it != g_fires.end();) {
		it->update();
		if (it->isOutOfScreen()) {
			it = g_fires.erase(it);
		}
		else {
			++it;
		}
	}
}

void Render() {
	// Background
	SDL_RenderCopy(g_renderer, g_bg_texture, &g_bg_source_rect, &g_bg_destination_rect);

	// Character
	SDL_Rect r = g_player_destination_rect;
	SDL_RenderCopy(g_renderer,
		g_player_sheet_texture,
		&g_player_source_rects[g_current_player_id],
		&r);

	// Fire
	for (const auto& fire : g_fires) {
		fire.render(g_renderer);
	}

	// Score
	RenderScore();

	SDL_RenderPresent(g_renderer);
}

void Set2Music() {
	// Set the music volume 
	Mix_VolumeMusic(12);

	music1_ = Mix_LoadMUS("../Resources/Zenith_Zone.mp3");
	if (!music1_)
	{
		printf(" %s\n", Mix_GetError());
		// this might be a critical error...
	}
	Mix_PlayMusic(music1_, -1);
}

void RenderScore() {
	string score_str = "점수: " + string(5 - to_string(g_score).length(), '0') + to_string(g_score);
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Surface* score_surface = TTF_RenderUTF8_Blended(game_font_, score_str.c_str(), white);
	text_score_ = SDL_CreateTextureFromSurface(g_renderer, score_surface);
	text_score_rect_.x = 40;
	text_score_rect_.y = 40;
	text_score_rect_.w = score_surface->w;
	text_score_rect_.h = score_surface->h;
	SDL_FreeSurface(score_surface);

	SDL_RenderCopy(g_renderer, text_score_, nullptr, &text_score_rect_);
	SDL_DestroyTexture(text_score_);
}


void ClearGame() {
	SDL_DestroyTexture(g_player_sheet_texture);
	SDL_DestroyTexture(g_bg_texture);

	for (auto& fire : g_fires) {
		fire.destroyTexture();
		fire.Clear2Sound();
	}
	g_fires.clear();
	Mix_FreeMusic(music1_);
	Mix_FreeChunk(wave1_);

	TTF_CloseFont(game_font_);
}