#pragma once

#include "Include.h"
#include "obj_Mahjong_Base.h"
#include "obj_Mahjong_Derived.h"
#include "obj_VFX.h"

class gameLogic {
public:
    //! 생성자 소멸자
    gameLogic();
    ~gameLogic();

    //! 기본 함수
    void HandleEvents();
    void Update();
    void Render();

    //! 최초 초기화 및 클리어 함수
    void InitGame();
    void ClearGame();
    void resetGame(); // 게임 재시작 함수 (gamePlay 에서 페이즈 변경될때 사용함)

    //! 마작 블록 생성 및 관리 함수
    void LoadMahjongBlocksFromCSV(int level, int seed, int numDims);
    void LoadMahjongBlocksIfEmpty(int level);
    
    //! 마작 블록 이동 및 정렬 함수
    void vector2stack(vector<unique_ptr<Mahjong>>::iterator it);
    void RemoveSameTypeBlocks();
    void AlignStackBlocks();
    bool checkEmptyBlocks();
    int countEmptyBlocks();
    bool compareBlocks(const unique_ptr<Mahjong>& a, const unique_ptr<Mahjong>& b);
    void quickSort(vector<unique_ptr<Mahjong>>& blocks, int left, int right);
    void sortPairedBlocks();

    //! 마작 플레이 영역 업데이트 함수
    void UpdateVectorBlocks();
    void UpdateStackBlocks();
    
    //! VFX 객체 생성 및 관리 함수
    void createBonk(int x, int y);
    void UpdateBonks();

    //! CSV 파일 관리 함수
    int countDir(const string& path);
    int countFiles(const string& path);

    //! 점수 및 게임상태 관련 함수
    int UpdateScore(int score);
    void printStatusChange();
    int checkMahjongType(string Type);

    //! Getters and Setters
    vector<unique_ptr<Mahjong>>& getVector() { return g_vector; }	/// g_vector 벡터 getter
    int getMaxLevel() const { return MAX_LEVEL; } 				    /// MAX_LEVEL getter
    int getLevel() const { return g_level; } 					    /// g_level getter
    int getMaxLevel() { return MAX_LEVEL; }						    /// MAX_LEVEL getter
    int getStatus() const { return g_status; }					    /// g_status 게임 상태 getter
    int getPrevStatus() const { return g_prevStatus; }			    /// g_prevStatus 이전 게임 상태 getter
    int getScore() const { return g_score; }					    /// m_score 이 pop되어 합산된 점수 모음 getter
    bool getIsPop() const { return isPop; }                         /// 마작 블록이 pop 되었는지 여부 getter

    vector<unique_ptr<Mahjong>>& getStack() { return g_stack; }     /// g_stack 벡터 getter
    void setMaxLevel(int maxLevel) { MAX_LEVEL = maxLevel; }		/// MAX_LEVEL setter
    void setLevel(int level) { g_level = level; }				    /// g_level setter
    void setStatus(int status) { g_status = status; }               /// g_status 게임 상태 설정 setter
    void setPrevStatus(int prevStatus) { g_prevStatus = prevStatus;}/// g_prevStatus 이전 게임 상태 setter
    void setScore(int score) { g_score = score; }				    /// m_score 이 pop되어 합산된 점수 모음 setter
    void setIsPop(bool value) { isPop = value; };                   /// 마작 블록이 pop 되었는지 여부 setter

private:
    vector<unique_ptr<Mahjong>> g_vector; // 마작 블록 생성 벡터
    vector<unique_ptr<Mahjong>> g_stack; // 마작 블록 스택 벡터
    vector<bonk> g_bonks; // 효과 블록 생성 벡터

    Mix_Chunk* wave1_;
    Mix_Music* music1_;

    SDL_Texture* text_score_;
    SDL_Rect text_score_rect_;
    TTF_Font* game_font_;

    SDL_Rect g_bg_source_rect;
    SDL_Rect g_bg_destination_rect;

    int g_level;        /// 현재 레벨
    int MAX_LEVEL;      /// 최대 레벨
    int g_status;       /// 게임 상태
    int g_prevStatus;   /// 이전 게임 상태
    int g_score;        /// m_score 이 pop되어 합산된 점수 모음
    bool isPop;         /// 마작 블록이 pop 되었는지 여부 (점수 반환을 위함)
};

//! ******************** 비멤버 함수 ********************
gameLogic* GameLogicInstance();                             //~ 게임 로직 인스턴스 생성 함수
Uint32 shakeBlocksCallback(Uint32 interval, void* param);   //~ 블록 흔들기 콜백 함수