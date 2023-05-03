#ifndef game_h
#define game_h

#include <SDL.h>
#include <string>
#include <vector>

#include "player.h"
class Map;
class raycaster;
class enemy;
class text;
#include "audio.h"

using namespace std;


struct render_info
{
    render_info(){};
    bool state = 1;
    int distance;
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Texture* texture;
};

class game
{
public:
    static bool lose;
    static bool  win;
    static SDL_Renderer *renderer;
    static const int FPS;
    static int second_passed; //veri intuitive time display
    static int clock_min;
    static int clock_sec;
    static bool all_coins;
    Uint32 start_time = 0;
    Uint32 pause_time = 0;
    Uint32 current_time;
    Uint32 delta_time;

    bool pause      = false;
    bool running    = true;
    bool playing    = true;

    vector <render_info> render_list;

    game(){};
    ~game(){};

    void init();

    void new_game();
    void game_loop();
    void userInput();
    void update();
    void render();
    void quit();

    void start_screen();
    void pause_menu();
    void lose_screen();
    void win_screen();
    void setting_menu();
    void warning();
    void reset();

    void item_gen();
    void item_update();

    void init_HUD();
    void update_HUD();
    void render_HUD();

    const int SCREEN_WIDTH  = 1280;
    const int SCREEN_HEIGHT = 720;

    SDL_Texture* LoadTexture(string path);

private:
    SDL_Event event;
    SDL_Window *window  = nullptr;
    const char* TITLE;
    int WINPOS_X        = SDL_WINDOWPOS_CENTERED;
    int WINPOS_Y        = SDL_WINDOWPOS_CENTERED;

    Map*            MAP;
    player*         PLAYER;
    raycaster*      RCAST;
    enemy*          ENEMY;


    game_audio* ammo_pickup = new game_audio("Resources/audio/ammo_pickup.wav",1,80);
    game_audio* coin_pickup = new game_audio("Resources/audio/coin_pickup.wav",1,80);
    int coin_count  = 10;
    int ammo_count  = 10;

    string font = "Resources/fonts/BRADHITC.TTF";
    text* coin_counter;
    text* ammo_counter;
    text* time_clock;
    SDL_Texture* gun_hud;
    SDL_Texture* coin_hud;
    SDL_Texture* back_ground;
};

#endif
