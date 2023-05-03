#ifndef player_h
#define player_h

#include <SDL.h>
#include <bits/stdc++.h>

#include "map.h"
#include "audio.h"
class enemy;
class asprites;

using namespace std;

class player
{
private:
    const float speed = 4;
    const int   width = 10, height = 10;
    float pos_x, pos_y;
    float step_x, step_y;
    float center_x, center_y;
    float strafe_x, strafe_y;
    float strafe_angle;
    float player_angle;
    SDL_Rect player_rect;

    //Items;
    int  ammo = 0;
    bool hit_enemy      = false;
    bool walking        = false;
    bool left_click     = false;
    bool FWD            = false,
         BWD            = false,
         LEFT           = false,
         RIGHT          = false;

    Map* mp;
    enemy* e;
    map<int,Uint32> doors;
    bool door           = false;
    bool escaped        = false;
    bool shooting       = false;


    int last_open;
    int frame_cnt = 0;
    Uint32 time   = SDL_GetTicks();

    asprites*   gun;
    game_audio* gun_shot;
    game_audio* door_open;
    game_audio* footstep;
    game_audio* empty_gun;
    game_audio* denied;
public:
    player(){};
    ~player(){}
    player(double x, double y);
    void update(enemy* &e);
    void render();
    void render2D();
    void movement();
    void mse_ctr(SDL_Event event);
    void kbd_ctr(SDL_Event event);
    void set_default();
    void get_map(Map* &mp);
    void picked_ammo(){ammo++;}
    int get_ammo(){return ammo;}
    bool check_escape(){return escaped;}
    float get_player_angle(){return player_angle;}
    float get_center_x()    {return center_x;}
    float get_center_y()    {return center_y;}
    SDL_Rect rect()         {return player_rect;}
};

#endif // player_h
