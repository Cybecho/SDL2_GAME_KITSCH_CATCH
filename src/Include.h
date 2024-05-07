#pragma once // 헤더가드 대신 pragma once 사용
#define MOVE_SPEED 20.0f // 이동 속도
#define WINDOW_WIDTH 540 // 창 너비
#define WINDOW_HEIGHT 960 // 창 높이

//! 기본 헤더
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <stack>
#include <cmath>
#include <ctime>
#include <atlstr.h>
#include <windows.h>

//! SDL 헤더
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

//! 타입 정의
// using 키워드를 사용하여 타입을 정의 aliasing 하는거임
using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;
using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

//! extern 변수
// extern 이란 변수를 선언만 하고 정의는 다른 곳에서 하는 것
extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern Uint32 g_frame_per_sec;
extern int g_input; // 사용자의 키를 입력받는 변수

//! using namespace
using namespace std;