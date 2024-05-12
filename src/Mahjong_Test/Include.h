// Include.h
#pragma once

#define WINDOW_WIDTH 540
#define WINDOW_HEIGHT 960
#define g_window_margin 20
#define BLOCK_SIZE 100
#define BLOCK_SCALE 1

enum GamePhase {
    PHASE_INTRO,
    PHASE_MAIN,
    PHASE_PLAYING,
    PHASE_ENDING_CLEAR,
    PHASE_ENDING_GAMEOVER,
    PHASE_PAUSE
};

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <stack>
#include <cmath>
#include <ctime>
#include <atlstr.h>
#include <windows.h>
#include <fstream>
#include <memory>

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
extern SDL_Renderer* g_renderer;
extern Uint32 g_frame_per_sec;
extern int g_input;

using namespace std;