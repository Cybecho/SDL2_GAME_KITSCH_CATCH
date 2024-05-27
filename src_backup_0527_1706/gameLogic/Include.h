// Include.h
//~ 해당 Inlcude 파일은 이제 사용되지 않습니다.
//~ ../Include.h 를 사용합니다.
#pragma once

#define WINDOW_WIDTH 540   // 윈도우 너비
#define WINDOW_HEIGHT 960  // 윈도우 높이
#define PIVOT_X 20         // 고정 피벗 x좌표
#define PIVOT_Y 440        // 고정 피벗 y좌표
#define g_window_margin 20 // 윈도우 마진
#define BLOCK_SIZE 100     // 블록 기본 크기
#define BLOCK_SCALE 1.0f   // 블록 크기 배율

enum GamePhase
{
    PHASE_INTRO,
    PHASE_MAIN,
    PHASE_PLAYING,
    PHASE_ENDING_CLEAR,
    PHASE_ENDING_GAMEOVER,
    PHASE_PAUSE
};

enum GameStatus
{
    STATUS_GAMEPLAYING,
    STATUS_GAMECLEAR,
    STATUS_GAMEOVER
};

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <queue>
#include <vector>
#include <stack>
#include <cmath>
#include <atlstr.h>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <memory>
#include <map>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;
using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

extern bool g_flag_running;
extern SDL_Renderer *g_renderer;
extern Uint32 g_frame_per_sec;
extern int g_input;

using namespace std;