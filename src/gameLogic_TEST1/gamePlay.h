#pragma once
#include "gameClass.h"
#include "obj_Mahjong_Base.h"
#include "obj_Mahjong_Derived.h"
#include "obj_VFX.h"
// play scene

class gamePlay : public gameClass
{
public:
    //! gamePlay
    gamePlay();
    virtual ~gamePlay();

    void play_timer(int interval);
    void changePhaseToEnding();
    void changePhaseToMain();
    void HandleEvents();
    void Update();
    void Render();
    void updateScore(int s);

    //! gameLogic
    

protected:
    //! gamePlay
    // const Uint32 p_start_time_ms;
    Uint32 p_last_time_ms;
    string string_time;
    string update_score;

    // int update_score; //new score
    // float timebarw;

    SDL_Texture *play_bg;
    SDL_Texture *playground_img;
    SDL_Texture *stack_img;
    SDL_Texture *timebar_bg;
    SDL_Texture *cat;
    SDL_Texture *cat2;
    SDL_Texture *setting_bt;
    SDL_Texture *setting;
    SDL_Texture *score_text2; // get score from txt file

    SDL_Rect playground_rect;
    SDL_Rect stack_rect;

    SDL_Rect cat_rect;
    SDL_Rect cat_rect2;
    SDL_Rect settingBT_rect;

    // score
    SDL_Rect exp_rect;
    SDL_Rect score_rect;

    Mix_Chunk *setting_SoundEffect;
    
    //! gameLogic
    //! 필수 함수
    //~ void HandleEvents();    // 중복 함수
    //~ void Update();          // 중복 함수
    //~ void Render();          // 중복 함수
    void InitGame();
    void ClearGame();

    //! 사용자 정의 함수
    void LoadMahjongBlocksFromCSV(int level, int seed, int numDims);
    void vector2stack(vector<unique_ptr<Mahjong>>::iterator it);
    void createBonk(int x, int y); // 추가된 함수 선언

    //! 업데이트 함수
    void LoadMahjongBlocksIfEmpty(int level);
    void RemoveSameTypeBlocks();
    void AlignStackBlocks();
    void UpdateVectorBlocks();
    void UpdateStackBlocks();
    void UpdateBonks();
    void UpdateScore(int score);

    //! Empty Block 체크 함수
    bool checkEmptyBlocks();
    int countEmptyBlocks();

    //! 정렬 함수
    bool compareBlocks(const unique_ptr<Mahjong>& a, const unique_ptr<Mahjong>& b);
    void quickSort(vector<unique_ptr<Mahjong>>& blocks, int left, int right);
    void sortPairedBlocks();
};

//! gamePlay
extern Mix_Music *main_music;
extern Mix_Music *play_music;
extern Mix_Music *clear_music;
extern bool cat_status;
extern bool clear_reset;

extern Uint32 g_last_time_ms;
extern bool isChanged;
extern bool isForcedQuit;

extern SDL_Rect timebar_rect;
extern int stage;
extern int count_;
extern int sec; // play second
extern int limit_sec;
extern int last_sec; // last second (stage second)

extern string original_score;
extern int org_score_int;
extern int plus_score_int; // load from txt file

//! gameLogic
extern vector<unique_ptr<Mahjong>> g_vector;
extern int g_blockCount;
extern int g_level;
extern int g_status; // 0 : playing , 1 : gameclear, 2 : gameover
extern int MAX_LEVEL;

//! 파일 세기 함수
int countDir(const string& path);
int countFiles(const string& path);