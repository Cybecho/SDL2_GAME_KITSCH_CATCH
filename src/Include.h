#pragma once                // 헤더가드 대신 pragma once 사용
#define MOVE_SPEED 20.0f    // 이동 속도
#define WINDOW_WIDTH 540    // 윈도우 너비
#define WINDOW_HEIGHT 960   // 윈도우 높이
#define PIVOT_X 20          // 고정 피벗 x좌표
#define PIVOT_Y 440         // 고정 피벗 y좌표
#define PIVOT_Y2 10 	    // 고정 피벗 y좌표 (vector2stack , Align Stack Block 에서 사용함)
#define g_window_margin 20  // 윈도우 마진
#define BLOCK_SIZE 100      // 블록 기본 크기
#define BLOCK_SCALE 1.0f    // 블록 크기 배율
#define MAX_STACK 7 	    // 최대 스택 블록 수
#define LIMIT_TIME 60       // 제한 시간
#define ADD_TIME 1 	        // 추가 시간

//! Game Phases Enum
enum GamePhase
{
    PHASE_INTRO,
    PHASE_MAIN,
    PHASE_PLAYING,
    PHASE_ENDING_CLEAR,
    PHASE_ENDING_GAMEOVER,
    PHASE_PAUSE
};

//! Game Status Enum
enum GameStatus
{
    STATUS_GAMEPLAYING,
    STATUS_GAMECLEAR,
    STATUS_GAMEOVER
};

//! Mahjong Type Enum
enum MahjongType {
    MahjongType_0,
    MahjongType_1,
    MahjongType_2,
    MahjongType_3
}

//! 기본 헤더
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
#include <time.h>

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
extern SDL_Renderer *g_renderer;
extern Uint32 g_frame_per_sec;
extern int g_input; // 사용자의 키를 입력받는 변수

//! using namespace
using namespace std;
