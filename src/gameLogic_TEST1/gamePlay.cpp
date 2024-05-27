#include "gamePlay.h"

extern SDL_Texture *exp_text; //"EXP" text

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

int g_level;
int MAX_LEVEL = countDir("../../res/level") - 1; // 최대 레벨 수는 디렉토리 개수 - 1 (for문 때문에 -1 처리를 해줌)
int g_status = STATUS_GAMEPLAYING;

gamePlay::gamePlay()
{
    InitGame();
}

gamePlay::~gamePlay()
{
    Mix_FreeMusic(play_music);
    Mix_FreeChunk(setting_SoundEffect);
    // SDL_DestroyTexture(exp_text);
    SDL_DestroyTexture(score_text2);
    SDL_DestroyTexture(play_bg);
    SDL_DestroyTexture(cat);
    SDL_DestroyTexture(cat2);
    SDL_DestroyTexture(setting);
    // SDL_DestroyTexture(setting_bt);
    // SDL_DestroyTexture(timebar_bg);
    // SDL_DestroyTexture(stack_img);
    // SDL_DestroyTexture(playground_img);
    SDL_Quit();
    TTF_Quit();

    ClearGame();
}

void gamePlay::HandleEvents()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            g_flag_running = false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                isChanged = true;
                isForcedQuit = true;
                SDL_Delay(33);

                changePhaseToEnding();
            }
            else
            {
                Mix_PlayMusic(play_music, -1);
            }
            break;

        default:
            break;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                // setting key
                if (isSetting == false && mouseX > setting_bt_rect.x && mouseY > setting_bt_rect.y &&
                    mouseX < setting_bt_rect.x + setting_bt_rect.w && mouseY < setting_bt_rect.y + setting_bt_rect.h)
                {
                    isSetting = !isSetting;
                    Mix_PlayChannel(-1, setting_SoundEffect, 0);
                }

                if (isSetting == true)
                {
                    if (mouseX > set_Xkey_rect.x && mouseY > set_Xkey_rect.y &&
                        mouseX < set_Xkey_rect.x + set_Xkey_rect.w && mouseY < set_Xkey_rect.y + set_Xkey_rect.h)
                    {
                        isSetting = !isSetting;
                        Mix_PlayChannel(-1, setting_SoundEffect, 0);
                    }

                    if (mouseX > volume_bt_rect.x && mouseY > volume_bt_rect.y &&
                        mouseX < volume_bt_rect.x + volume_bt_rect.w && mouseY < volume_bt_rect.y + volume_bt_rect.h)
                    {
                        isVolumeOff = !isVolumeOff;
                        Mix_PlayChannel(-1, setting_SoundEffect, 0);
                        if (isVolumeOff == true)
                        {
                            Mix_PauseMusic();
                        }
                        else if (isVolumeOff == false)
                        {
                            Mix_ResumeMusic();
                        }
                    }

                    // resume
                    if (mouseX > resume_rect.x && mouseY > resume_rect.y &&
                        mouseX < resume_rect.x + resume_rect.w && mouseY < resume_rect.y + resume_rect.h)
                    {
                        isSetting = !isSetting;
                        Mix_PlayChannel(-1, setting_SoundEffect, 0);
                    }

                    // go to home
                    if (mouseX > home_rect.x && mouseY > home_rect.y &&
                        mouseX < home_rect.x + home_rect.w && mouseY < home_rect.y + home_rect.h)
                    {
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

                        changePhaseToMain();
                    }
                }
            }
            else if (isSetting == false && event.button.button == SDL_BUTTON_RIGHT)
            {
                plus_score_int += 10;
            }
        }

        //! 스페이스바 누르면 벡터&스택 초기화
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == 'r') {
            g_vector.clear();
            g_stack.clear();
        }

        if (event.type == SDL_MOUSEMOTION) {
            int mouse_x = event.motion.x;
            int mouse_y = event.motion.y;

            for (auto& block : g_vector) {
                if (block->isClickable()) {
                    block->setHovered(block->isHovered(mouse_x, mouse_y));
                }
            }
        }

        //! 마우스 클릭 이벤트 처리
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouse_x = event.button.x;
            int mouse_y = event.button.y;

            // g_vector를 역순으로 순회하여 상위 레이어부터 클릭 이벤트 처리
            for (auto it = g_vector.rbegin(); it != g_vector.rend(); ++it) {
                //! clickEnable이 true이고 Mahjong_Empty 블록이 아닌 경우에만 클릭 이벤트 처리
                if ((*it)->isClickable() && !dynamic_cast<Mahjong_Empty*>(it->get()) && (*it)->isClicked(mouse_x, mouse_y)) {
                    (*it)->Play2Sound();

                    vector2stack(it.base() - 1); //! 클릭된 블록을 g_stack으로 이동
                    createBonk(mouse_x, mouse_y); //! 클릭 위치에 bonk 객체 생성
                    break;
                }
            }
        }
    }
}

void gamePlay::Update()
{
    switch (stage)
    {
    default:
        limit_sec = 10;
        break;
    }
    string score;
    updateScore(plus_score_int);

    if (isForcedQuit)
    {
        isForcedQuit = false;
    }

    if (isChanged == false)
    {

        if (isSetting)
        {
        }
        else
        {

            count_ += 1;
            if (count_ != 0 && count_ % 33 == 0)
            {
                sec += 1; // play second
                timebar_rect.w = timebar_rect.w - 540 / limit_sec;

                if (last_sec == 0)
                {
                    SDL_Delay(1000);
                    isChanged = true;
                    // isForcedQuit = true;
                    changePhaseToEnding();
                }
            }
        }
    }
    else if (isChanged)
    {
        timebar_rect.w = 540;
    }

    last_sec = limit_sec - sec;

    std::cout << "��� �ð� : " << sec << " ���� �ð� : " << last_sec << std::endl;

    LoadMahjongBlocksIfEmpty(g_level);
    RemoveSameTypeBlocks();
    AlignStackBlocks();
    UpdateVectorBlocks();
    UpdateStackBlocks();
    UpdateBonks();
}

void gamePlay::Render()
{
    SDL_RenderCopy(g_renderer, play_bg, NULL, NULL);

    // time bar
    SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255); // red
    SDL_RenderFillRect(g_renderer, &timebar_rect);

    if (cat_status == false)
    {
        // cat image 1
        SDL_Rect tmp_r;
        tmp_r.x = 135;
        tmp_r.y = -110;
        tmp_r.w = cat_rect.w * 0.45;
        tmp_r.h = cat_rect.h * 0.45;
        SDL_RenderCopy(g_renderer, cat, &cat_rect, &tmp_r);
    }
    else
    {
        // cat image 2
        SDL_Rect tmp_r1;
        tmp_r1.x = 135;
        tmp_r1.y = -110;
        tmp_r1.w = cat_rect2.w * 0.45;
        tmp_r1.h = cat_rect2.h * 0.45;
        SDL_RenderCopy(g_renderer, cat2, &cat_rect2, &tmp_r1);
    }

    { // score text
        SDL_Rect tmp_r;
        tmp_r.x = g_window_margin + 80;
        tmp_r.y = 37;
        tmp_r.w = score_rect.w * 0.8;
        tmp_r.h = score_rect.h * 0.8;
        SDL_RenderCopy(g_renderer, score_text2, &score_rect, &tmp_r);
    }

    if (isSetting == true)
    {
        SDL_Rect tmp_r;
        tmp_r.x = 0;
        tmp_r.y = 0;
        tmp_r.w = setting_rect.w;
        tmp_r.h = setting_rect.h;
        SDL_RenderCopy(g_renderer, setting, &setting_rect, &tmp_r);

        if (isVolumeOff == false)
        {
            SDL_RenderCopy(g_renderer, volume_bt_off, &volume_rect, &volume_rect);
        }
        else if (isVolumeOff == true)
        {
            SDL_RenderCopy(g_renderer, volume_bt_on, &volume_rect, &volume_rect);
        }
    }

    // 블록 객체 렌더링
    for (const auto& block : g_vector) {
        block->render(g_renderer);
    }
    for (const auto& block : g_stack) {
        block->render(g_renderer);
    }

    // bonk 객체 렌더링
    for (const auto& bonk : g_bonks) {
        bonk.render(g_renderer);
    }

    SDL_RenderPresent(g_renderer);
}

void gamePlay::InitGame()
{
    isForcedQuit = false;
    isChanged = false;
    sec = 0;
    count_ = 0;
    stage = 1;
    isSetting = false;
    isVolumeOff = false;

    g_flag_running = true;
    g_status = STATUS_GAMEPLAYING;
    g_level = 0;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cout << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return;
    }

    // Initialize SDL_ttf
    if (TTF_Init() < 0)
    {
        std::cout << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        return;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "SDL_mixer initialization failed: " << Mix_GetError() << std::endl;
        return;
    }

    // Create window
    g_window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_window == nullptr)
    {
        std::cout << "Window creation failed: " << SDL_GetError() << std::endl;
        return;
    }

    // Create renderer
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (g_renderer == nullptr)
    {
        std::cout << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return;
    }

    // BG
    SDL_Surface* bg_surface = IMG_Load("../../res/game page/game_background.png");
    play_bg = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
    SDL_FreeSurface(bg_surface);

    g_bg_source_rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    g_bg_destination_rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    // cat
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

    { // time bar
        timebar_rect.x = 0;
        timebar_rect.y = 290;
        timebar_rect.w = 540;
        timebar_rect.h = g_window_margin;
    }

    plus_score_int = org_score_int;

    { // setting img
        // mainscreen setting
        SDL_Surface* set_surface = IMG_Load("../../res/setting/setting_background.png");
        setting = SDL_CreateTextureFromSurface(g_renderer, set_surface);

        setting_rect.x = 0;
        setting_rect.y = 0;
        setting_rect.w = set_surface->w;
        setting_rect.h = set_surface->h;

        SDL_FreeSurface(set_surface);
    }

    // music
    play_music = Mix_LoadMUS("../../res/testRes/testBGM3.mp3");
    if (play_music == 0)
    {
        std::cout << "Mix_LoadMUS(\"testBGM2.mp3\"): " << Mix_GetError() << std::endl;
    }

    setting_SoundEffect = Mix_LoadWAV("../../res/testRes/testSound.mp3");
}

void gamePlay::changePhaseToEnding()
{
    g_current_game_phase = PHASE_ENDING_CLEAR;
    clear_reset = true;
    Mix_HaltMusic();
    Mix_PlayMusic(clear_music, -1);
}

void gamePlay::changePhaseToMain()
{
    g_current_game_phase = PHASE_MAIN;
    Mix_HaltMusic();
    Mix_PlayMusic(main_music, -1);
}

void gamePlay::play_timer(int interval)
{
    // 90s = 90000
    //
    // const Uint32* pointer = &g_last_time_ms;
}

void gamePlay::updateScore(int s)
{
    string front_score;
    update_score = std::to_string(s);

    // ���� ���ڸ����� ����
    if (s == 0)
    {
        front_score = "000";
        update_score = front_score + update_score;
    }
    else if (s > 0 && s < 10)
    {
        front_score = "000";
        update_score = front_score + update_score;
    }
    else if (s >= 10 && s < 100)
    {
        front_score = "00";
        update_score = front_score + update_score;
    }
    else if (s >= 100 && s < 1000)
    {
        front_score = "0";
        update_score = front_score + update_score;
    }
    else
    {
        update_score = std::to_string(s);
    }

    TTF_Font *font = TTF_OpenFont("../../res/testRes/Galmuri14.ttf", 30);
    SDL_Color white = {255, 255, 255, 0};
    SDL_Surface *tmp_surface = TTF_RenderUTF8_Blended(font, update_score.c_str(), white);
    // std::to_string(score).c_str()
    score_rect.x = 0;
    score_rect.y = 0;
    score_rect.w = tmp_surface->w;
    score_rect.h = tmp_surface->h;

    score_text2 = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
    SDL_FreeSurface(tmp_surface);
    TTF_CloseFont(font);
}

//! **************** gameLogic ****************

//! CSV 파일에서 마작 블록을 읽어와서 g_vector에 생성
void gamePlay::LoadMahjongBlocksFromCSV(int level, int seed, int numDims) {
    g_vector.clear();

    for (int dim = 0; dim <= numDims; ++dim) {
        string filename = "../../res/level/" + to_string(level) + "/" + to_string(seed) + "-" + to_string(dim) + ".csv";
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Failed to open file: " << filename << endl;
            continue;
        }
        string line;
        int row = 0;

        while (getline(file, line)) {
            istringstream iss(line);
            string cell;
            int col = 0;

            while (getline(iss, cell, ',')) {
                int data = stoi(cell);

                int x = col * BLOCK_SIZE + PIVOT_X + (dim * BLOCK_SIZE / 2);
                int y = row * BLOCK_SIZE + PIVOT_Y + (dim * BLOCK_SIZE / 2);

                if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) {
                    switch (data) {
                    case 1:
                        g_vector.emplace_back(make_unique<Mahjong_A>(x, y, g_renderer));
                        g_vector.back()->setPosition(dim, row, col);
                        break;
                    case 2:
                        g_vector.emplace_back(make_unique<Mahjong_B>(x, y, g_renderer));
                        g_vector.back()->setPosition(dim, row, col);
                        break;
                    case 3:
                        g_vector.emplace_back(make_unique<Mahjong_C>(x, y, g_renderer));
                        g_vector.back()->setPosition(dim, row, col);
                        break;
                    case 4:
                        g_vector.emplace_back(make_unique<Mahjong_D>(x, y, g_renderer));
                        g_vector.back()->setPosition(dim, row, col);
                        break;
                    default:
                        g_vector.emplace_back(make_unique<Mahjong_Empty>(x, y, g_renderer));
                        g_vector.back()->setPosition(dim, row, col);
                        break;
                    }
                }

                ++col;
            }

            ++row;
            cout << endl; // 줄바꿈
        }
    }

    //! 대각선 방향으로 블록 흔들림 효과 적용
    int totalBlocks = g_vector.size();
    for (int i = 0; i < totalBlocks; ++i) {
        int delay = i * 3; // 0.03초(3ms) 지연 시간
        SDL_AddTimer(delay, [](Uint32 interval, void* param) -> Uint32 {
            int index = *(static_cast<int*>(param));
            g_vector[index]->shakeBlocks(5); // 애니메이션 지속시간
            delete static_cast<int*>(param);
            return 0;
            }, new int(i));
    }
}

//! 클릭된 블록을 g_vector에서 g_stack으로 이동
void gamePlay::vector2stack(vector<unique_ptr<Mahjong>>::iterator it) {
    auto block = move(*it);
    g_vector.erase(it);

    // 객체의 위치를 g_stack의 크기에 따라 동적으로 계산합니다.
    int x = g_stack.size() * (BLOCK_SIZE / 2) + PIVOT_X;
    int y = BLOCK_SIZE + PIVOT_Y - (BLOCK_SIZE * 2);

    block->setX(x);
    block->setY(y);

    g_stack.emplace_back(move(block));

    // g_blocks와 g_stack의 사이즈 출력
    cout << "g_blocks size: " << g_vector.size() - countEmptyBlocks() << "| g_stack size: " << g_stack.size() << endl;
}

//! 클릭된 위치에 bonk 객체 생성
void gamePlay::createBonk(int x, int y) {
    if (BLOCK_SCALE == 1) {
        g_bonks.emplace_back(x - (BLOCK_SIZE / 2), y - (BLOCK_SIZE / 2), g_renderer);
    }
    else {
        g_bonks.emplace_back(x - (BLOCK_SIZE / (BLOCK_SCALE * BLOCK_SCALE)), y - (BLOCK_SIZE / (BLOCK_SCALE * BLOCK_SCALE)), g_renderer);
    }
}

//! update 함수 : 벡터 영역이 비었을 때 csv 파일을 읽어와서 다시 로드
void gamePlay::LoadMahjongBlocksIfEmpty(int level) {
    if (checkEmptyBlocks()) {
        for (auto it = g_vector.begin(); it != g_vector.end();) {
            if (dynamic_cast<Mahjong_Empty*>(it->get())) {
                it = g_vector.erase(it);
            }
            else {
                ++it;
            }
        }

        //~ 레벨 증가 로직
        if (level < MAX_LEVEL) { g_level++; }   //~ g_level이 최대 레벨이 아닐 경우 레벨 증가
        else { g_status = STATUS_GAMECLEAR; }       //~ g_level이 최대 레벨일 경우 게임오버
        srand(time(NULL));
        string dir_path = "../../res/level/" + to_string(level);
        int seed = rand() % (countFiles(dir_path) / 3);  //~ 현재 레벨에서 발생 가능한 시드 (전체파일수 / 3)
        int numDims = 2;
        LoadMahjongBlocksFromCSV(level, seed, numDims);
    }
}

//! update 함수 :  g_stack에서 같은 객체가 3개 이상 존재할 경우 pop 처리
void gamePlay::RemoveSameTypeBlocks() {
    map<string, vector<int>> typeIndices;
    for (int i = 0; i < g_stack.size(); ++i) {
        if (dynamic_cast<Mahjong_Empty*>(g_stack[i].get()) == nullptr) {
            typeIndices[g_stack[i]->getType()].push_back(i);
        }
    }

    bool blocksRemoved = false;

    for (auto it = typeIndices.rbegin(); it != typeIndices.rend(); ++it) {
        if (it->second.size() >= 3) {
            int count = 0;
            for (int i = it->second.size() - 1; i >= 0; --i) {
                if (count < 3) {
                    int index = it->second[i];
                    // 제거되는 블록의 위치에서 createBonk() 호출
                    int x = g_stack[index]->getX() + BLOCK_SIZE / 2;
                    int y = g_stack[index]->getY() + BLOCK_SIZE / 2;
                    createBonk(x, y);
                    g_stack.erase(g_stack.begin() + index);
                    ++count;
                    blocksRemoved = true;
                }
                else {
                    break;
                }
            }
        }
    }

    // 블록이 제거된 경우에만 shakeBlocks() 호출
    if (blocksRemoved) {
        for (auto& block : g_stack) {
            block->shakeBlocks(10);
        }
    }
}

//! update 함수 :  g_stack에 남은 객체들 순서대로 위치 조정
void gamePlay::AlignStackBlocks() {
    sortPairedBlocks(); //~ 쌍이 2개 이상인 객체들만 정렬

    for (size_t i = 0; i < g_stack.size(); ++i) {
        int x = i * (BLOCK_SIZE / 2 + 15) + PIVOT_X;
        int y = BLOCK_SIZE + PIVOT_Y - (BLOCK_SIZE * 2);
        g_stack[i]->setX(x);
        g_stack[i]->setY(y);
        g_stack[i]->setHovered(false); //~ 호버링 효과 제거 (이렇게 해줘야 크기 g_stack에서 이상하게 안변함)
    }
}

//! update 함수 :  g_stack의 객체들이 7개가 되면 g_vector의 객체들을 비활성화
void gamePlay::UpdateVectorBlocks()
{
    for (auto& block : g_vector)
    {
        block->handleClick();
        block->update();
        block->checkClickEnable(g_vector); // g_vector를 매개변수로 전달합니다.

        if (g_stack.size() == 7)
        {
            block->setClickable(false);
            g_status = STATUS_GAMEOVER;
        }
    }
}

//! update 함수 :  g_stack의 객체들이 7개가 되면 g_stack의 객체들을 비활성화
void gamePlay::UpdateStackBlocks() {
    for (auto& block : g_stack) {
        block->handleClick();
        block->update();
    }
}

//! update 함수 :  bonk 객체 업데이트
void gamePlay::UpdateBonks() {
    for (auto it = g_bonks.begin(); it != g_bonks.end();) {
        it->update();
        if (it->isTimeToDestroy()) {
            it = g_bonks.erase(it);
        }
        else {
            ++it;
        }
    }
}

//! update 함수 :  점수 업데이트
//~ 미구현
void gamePlay::UpdateScore(int score) {
}

//! Empty 마작 블록이 비었는지 체크
bool gamePlay::checkEmptyBlocks() {
    //~ g_vector가 비어있거나 모든 블록이 Empty 블록인 경우 true 반환
    return g_vector.empty() || all_of(g_vector.begin(), g_vector.end(), [](const auto& block) {
        return dynamic_cast<Mahjong_Empty*>(block.get()) != nullptr; //~ Empty 블록인지 체크
        });
}

//! Empty 마작 블록 개수를 세는 함수
int gamePlay::countEmptyBlocks() {
    return count_if(g_vector.begin(), g_vector.end(), [](const auto& block) {
        return dynamic_cast<Mahjong_Empty*>(block.get()) != nullptr;
        });
}

//! Sort 함수 : 퀵 정렬을 위한 비교 함수
bool gamePlay::compareBlocks(const unique_ptr<Mahjong>& a, const unique_ptr<Mahjong>& b) {
    if (a->getType() == b->getType()) {
        return a->getX() < b->getX();
    }
    return a->getType() < b->getType();
}

//! Sort 함수 : 퀵 정렬 함수
void gamePlay::quickSort(vector<unique_ptr<Mahjong>>& blocks, int left, int right) {
    if (left >= right) {
        return;
    }

    int pivot = left;
    int i = left + 1;
    int j = right;

    while (i <= j) {
        while (i <= right && compareBlocks(blocks[i], blocks[pivot])) {
            i++;
        }
        while (j > left && !compareBlocks(blocks[j], blocks[pivot])) {
            j--;
        }

        if (i > j) {
            swap(blocks[pivot], blocks[j]);
        }
        else {
            swap(blocks[i], blocks[j]);
        }
    }

    quickSort(blocks, left, j - 1);
    quickSort(blocks, j + 1, right);
}

//! Sort 함수 : 쌍이 2개 이상인 객체들만 정렬하는 함수
void gamePlay::sortPairedBlocks() {
    map<string, int> typeCount;
    for (const auto& block : g_stack) {
        typeCount[block->getType()]++;
    }

    vector<unique_ptr<Mahjong>> pairedBlocks;
    vector<unique_ptr<Mahjong>> singleBlocks;

    for (auto& block : g_stack) {
        if (typeCount[block->getType()] >= 2) {
            pairedBlocks.push_back(move(block));
        }
        else {
            singleBlocks.push_back(move(block));
        }
    }

    quickSort(pairedBlocks, 0, pairedBlocks.size() - 1);

    g_stack.clear();
    for (auto& block : pairedBlocks) {
        g_stack.push_back(move(block));
    }
    for (auto& block : singleBlocks) {
        g_stack.push_back(move(block));
    }
}

//! 게임 종료 시 메모리 꼭! 해제
void gamePlay::ClearGame()
{
    for (auto& block : g_vector) {
        block->destroyTexture();
        block->Clear2Sound();
    }
    g_vector.clear();
    for (auto& block : g_stack) {
        block->destroyTexture();
        block->Clear2Sound();
    }
    g_vector.clear();
    g_stack.clear();
    g_bonks.clear();

    Mix_FreeMusic(music1_);
    Mix_FreeChunk(wave1_);

    TTF_CloseFont(game_font_);

    Mix_FreeMusic(play_music);
    Mix_FreeChunk(setting_SoundEffect);
    // SDL_DestroyTexture(exp_text);
    SDL_DestroyTexture(score_text2);
    SDL_DestroyTexture(play_bg);
    SDL_DestroyTexture(cat);
    SDL_DestroyTexture(cat2);
    SDL_DestroyTexture(setting);

    // Destroy renderer and window
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);

    // Quit SDL subsystems
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

//! 디렉토리 내 폴더 개수 세기
int countDir(const string& path) {
    int count = 0;
    wstring search_path = wstring(path.begin(), path.end()) + L"\\*";
    WIN32_FIND_DATAW fd;
    HANDLE hFind = FindFirstFileW(search_path.c_str(), &fd);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                (wcscmp(fd.cFileName, L".") != 0) &&
                (wcscmp(fd.cFileName, L"..") != 0)) {
                ++count;
            }
        } while (FindNextFileW(hFind, &fd));
        FindClose(hFind);
    }

    cout << path << " dir count: " << count << endl;
    return count;
}

//! 디렉토리 내 파일 개수 세기
int countFiles(const string& path) {
    int count = 0;
    wstring search_path = wstring(path.begin(), path.end()) + L"\\*";
    WIN32_FIND_DATAW fd;
    HANDLE hFind = FindFirstFileW(search_path.c_str(), &fd);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                ++count;
            }
        } while (FindNextFileW(hFind, &fd));
        FindClose(hFind);
    }
    cout << path << " csv count: " << count << endl;
    return count;
}

