#include "game.h"
#include "player.h"
#include "map.h"
#include "raycaster.h"
#include "sprite.h"
#include "enemy.h"
#include "coin.h"
#include "ammo.h"
#include "text.h"

#include <bits/stdc++.h>
#include <iostream>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL.h>
#include <vector>

using namespace std;

vector<coin>    COINS;
vector<ammo>    AMMO;

bool game::lose = false;
bool game::win  = false;
SDL_Renderer* game::renderer = nullptr;
int game::second_passed = 0;
int game::clock_min = 0;
int game::clock_sec = 0;
bool game::all_coins  = false;
void game::init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) std::cout << "Init error " << SDL_GetError() << endl;
    else
    {
        window = SDL_CreateWindow(TITLE,WINPOS_X,WINPOS_Y,SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == nullptr) cout << "Cannot create window " << SDL_GetError() << endl;
        renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        if(renderer == nullptr) cout << "Cannot create renderer " << SDL_GetError() << endl;
        SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP,"1",SDL_HINT_OVERRIDE);
    }
    if(TTF_Init() == 1) cout << "Cannot load TTF " << SDL_GetError() << endl;
    SDL_Init(SDL_INIT_AUDIO);
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0) cout << "Cannot init SDL_Mixer " << Mix_GetError();
}



bool render_order(render_info a, render_info b)
{
    return a.distance > b.distance;
}

void game::game_loop()
{
    reset();
    new_game();
    SDL_SetRelativeMouseMode(SDL_TRUE);

    while(playing)
    {
        update();
        render();
        userInput();

        if     (lose) lose_screen();
        else if(win) win_screen();
    }

    //if lose->main_menu->playing = false;
    //if lose->restart  -> break;
}

void game::new_game()
{
    playing = true;
    start_time = SDL_GetTicks();
    MAP     = new Map("Resources/map/map3.txt");

    PLAYER  = new player(100,100);
    PLAYER->get_map(MAP);

    RCAST   = new raycaster;
    RCAST->get_map(MAP);
    RCAST->get_player(PLAYER);

    ENEMY   = new enemy(960,960,500,500,PLAYER,"Resources/entities/teehee.png");

    item_gen();
    init_HUD();
}

void game::reset()
{
    clock_min = 0;
    clock_sec = 0;
    second_passed  = 0;
    all_coins      = false;
    coin_count = 10;
    ammo_count = 10;
    AMMO.clear();
    COINS.clear();
    render_list.clear();

    delete coin_counter;
    delete ammo_counter;
    delete time_clock;
    delete MAP;
    delete PLAYER;
    delete RCAST;
    delete ENEMY;

    MAP    = nullptr;
    PLAYER = nullptr;
    RCAST  = nullptr;
    ENEMY  = nullptr;
    coin_counter = nullptr;
    ammo_counter = nullptr;
    time_clock   = nullptr;

    lose = false;
    win  = false;
}

void game::update()
{
    current_time    = SDL_GetTicks();
    delta_time      = current_time - start_time;
    clock_min       = (delta_time/1000)/60;
    clock_sec       = (delta_time/1000)%60;
    second_passed   = clock_min*60+clock_sec;
    PLAYER->update(ENEMY);
    ENEMY ->update(PLAYER,MAP);
    item_update();
    update_HUD();
    //Check win or lose
    if(ENEMY->player_collide(PLAYER) && !ENEMY->get_state()) lose = true;
    else if(PLAYER->check_escape())          win  = true;

}

void game::render()
{
    SDL_RenderClear     (renderer);
    for(int i = 0; i < COINS.size();i++) COINS[i].render(render_list);
    for(int i = 0; i < AMMO.size() ;i++) AMMO[i].render(render_list);
    RCAST   ->cast_rays (render_list);
    ENEMY   ->render    (render_list);

    int n = render_list.size();
    sort(render_list.begin(),render_list.end(),render_order);
    for(int i = 0; i < n; i++) SDL_RenderCopy(renderer,render_list[i].texture,&render_list[i].src,&render_list[i].dst);
    render_list.clear();

    //MAP     ->Map_Render2D();
    PLAYER  ->render();
    render_HUD();
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
}

void game::quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void game::userInput()
{
    while(SDL_PollEvent(&event) != 0)
    {
        if(event.type == SDL_QUIT)
        {
            cout << "quit";
        }
        else
        {
            PLAYER->kbd_ctr(event);
            PLAYER->mse_ctr(event);
        }
        if(event.type== SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                pause_menu();
            }
        }
    }
}

void game::item_gen()
{
    int n = MAP->Map_Width*MAP->Map_Height;
    vector<int> open_spots;

    for(int i = 0; i < n; i++)
    {
        if(MAP->Map_Tilemap[i] == 0)
            open_spots.push_back(i);
    }

    for(int i = 0; i < coin_count; i++)
    {
        int t = rand()%open_spots.size();
        int place = open_spots[t];
        coin tmp(MAP->Map_DstTile[place].x,MAP->Map_DstTile[place].y+16,250,250,PLAYER,"Resources/entities/coin.png");
        COINS.push_back(tmp);
        open_spots.erase(open_spots.begin()+t);
    }

    for(int i = 0; i < ammo_count; i++)
    {
        int t = rand()%open_spots.size();
        int place = open_spots[t];
        ammo tmp(MAP->Map_DstTile[place].x,MAP->Map_DstTile[place].y+16,250,250,PLAYER,"Resources/entities/ammo.png");
        AMMO.push_back(tmp);
        open_spots.erase(open_spots.begin()+t);
    }
}

void game::item_update()
{
    for(int i = 0; i < COINS.size(); i++)
    {
        COINS[i].update(PLAYER);
        if(COINS[i].get_state() == false)
        {
            COINS.erase(COINS.begin()+i);
            cout << COINS.size() << endl;
            coin_pickup->play();
        }
    }

    if(COINS.empty())
    {
        all_coins = true;
    }
    for(int i = 0; i < AMMO.size(); i++)
    {
        AMMO[i].update(PLAYER);
        if(AMMO[i].get_state() == false)
        {
            AMMO.erase(AMMO.begin()+i);
            PLAYER->picked_ammo();
            cout << AMMO.size() << endl;
            ammo_pickup->play();
        }
    }
}

void game::init_HUD()
{
    SDL_Color white = {255,255,255,255};
    string coin_cnt = to_string(coin_count-COINS.size()) + "/" + to_string(coin_count);
    coin_counter    = new text(100,SCREEN_WIDTH/2,0,white,font,coin_cnt);
    coin_counter    ->set_x(SCREEN_WIDTH-coin_counter->rect().w-50);
    ammo_counter    = new text(100,SCREEN_WIDTH-320,SCREEN_HEIGHT-100,white,font,to_string(PLAYER->get_ammo()));
    time_clock      = new text(50,0,0,white,font,"00:00");
    gun_hud         = LoadTexture("Resources/entities/hud_shotgun.png");
    coin_hud        = LoadTexture("Resources/entities/coin.png");
}

void game::update_HUD()
{
    string coin_cnt = to_string(coin_count-COINS.size()) + "/" + to_string(coin_count);
    coin_counter->set_text(coin_cnt);
    ammo_counter->set_text(to_string(PLAYER->get_ammo()));

    string min_text, sec_text;
    if(clock_min < 10)
    {
        min_text = "0"+to_string(clock_min);
    }
    else min_text = to_string(clock_min);

    if(clock_sec < 10)
    {
        sec_text = "0"+to_string(clock_sec);
    }
    else sec_text = to_string(clock_sec);
    string watch_text = min_text + ":" + sec_text;
    time_clock->set_text(watch_text);

}

void game::render_HUD()
{
    coin_counter->render(renderer);
    ammo_counter->render(renderer);
    time_clock  ->render(renderer);
    SDL_Rect coin_rect  = {SCREEN_WIDTH - coin_counter->rect().w-200, 0, 100,100};
    SDL_Rect gun_rect;
    SDL_QueryTexture(gun_hud,NULL,NULL,&gun_rect.w,&gun_rect.h);
    gun_rect.w/=5;
    gun_rect.h/=5;
    gun_rect.x = SCREEN_WIDTH-gun_rect.w-20;
    gun_rect.y = SCREEN_HEIGHT-gun_rect.h-20;

    SDL_RenderCopy(renderer,coin_hud,NULL,&coin_rect);
    SDL_RenderCopy(renderer,gun_hud, NULL,&gun_rect);
}

SDL_Texture* game::LoadTexture(string path)
{
    SDL_Surface* loaded_Surface = IMG_Load(path.c_str());
    SDL_Texture* loaded_Texture = SDL_CreateTextureFromSurface(renderer, loaded_Surface);
    if(loaded_Surface == nullptr)
    {
        cout << "Unable to load map surface" << endl;
    }
    if(loaded_Texture == nullptr)
        cout << "Unable to load map texture" << endl;
    return loaded_Texture;
}
