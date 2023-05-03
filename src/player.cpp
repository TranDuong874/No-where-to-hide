#include "player.h"
#include "game.h"
#include "map.h"
#include "config.h"

#include <SDL.h>
#include <math.h>

#define PI 3.14159265359
#define PI2 PI/2
#define PI3 3*PI/2
#define D 0.0174533
#include "enemy.h"

#include "audio.h"
#include "animated_sprite.h"

player::player(double x, double y)
{
    pos_x = x;
    pos_y = y;

    player_rect.x   = pos_x;
    player_rect.y   = pos_y;
    player_rect.w   = width;
    player_rect.h   = height;
    player_angle    = PI/2;
    strafe_angle    = player_angle + PI/2;

    step_x = cos(player_angle)*speed;
    step_y = sin(player_angle)*speed;

    gun_shot  = new game_audio  ("Resources/audio/doom.wav",1,50);
    door_open = new game_audio  ("Resources/audio/door.wav",1,80);
    footstep  = new game_audio  ("Resources/audio/walk2.mp3",0,70);
    empty_gun = new game_audio  ("Resources/audio/empty.wav",1,50);
    denied    = new game_audio  ("Resources/audio/denied.wav",1,80);
    gun       = new asprites    ("shotgun2.png");


    gun->set_dst(1280/2-(127*3)/2,768-208*3,127*3,208*3);
    gun->add(12,2,1,1);
    gun->add(12,2,1,12);
}

void player::get_map(Map* &mp)
{
    this->mp = mp;
    for(int i = 0; i < mp->Map_Size; i++)
    {
        if(mp->Map_Tilemap[i] == 2) doors[i] = 0;
    }
}

void player::update(enemy* &e)
{
    time = SDL_GetTicks();
    if(left_click && ammo > 0)
    {
        if(e->check_in_sight() && e->in_hit_zone() )
        {
            e->set_speed(0);
        }
    }
    movement();
}

void player::render()
{
    if(left_click && ammo > 0)
    {
        if(shooting == false)
        {
            gun_shot->play();
            shooting = true;
        }
        if(gun->play_animation(1))
        {
            left_click = false;
            ammo--;
            shooting = false;
        }
    }
    else
    {
        if(left_click && ammo == 0)
        {
            empty_gun->play();
        }
        gun->play_animation(0);
        left_click = false;
    }
}

void player::render2D()
{
    SDL_SetRenderDrawColor(game::renderer,255,0,0,255);
    SDL_RenderFillRect(game::renderer,&player_rect);
    SDL_RenderDrawRect(game::renderer, &player_rect);
    SDL_RenderDrawLine(game::renderer,center_x,center_y,center_x+20*step_x,center_y+20*step_y);
}

void player::movement()
{
    int xo  = 0, yo  = 0;
    int sxo = 0, syo = 0;
    if(step_x < 0)   xo = -10;  else xo = 20;
    if(step_y < 0)   yo = -10;  else yo = 20;
    if(strafe_x < 0) sxo = -10; else sxo = 20;
    if(strafe_y < 0) syo = -10; else syo = 20;

    int ipx         = pos_x/mp->tileSize,
        ipx_add_xo  = (pos_x+xo)/mp->tileSize,
        ipx_sub_xo  = (pos_x-xo)/mp->tileSize;
    int ipy         = pos_y/mp->tileSize,
        ipy_add_yo  = (pos_y+yo)/mp->tileSize,
        ipy_sub_yo  = (pos_y-yo)/mp->tileSize;
    int sipx        = pos_x/mp->tileSize,
        sipx_add_xo = (pos_x+sxo)/mp->tileSize,
        sipx_sub_xo = (pos_x-sxo)/mp->tileSize;
    int sipy        = pos_y/mp->tileSize,
        sipy_add_yo = (pos_y+syo)/mp->tileSize,
        sipy_sub_yo = (pos_y-syo)/mp->tileSize;

    if(FWD)
    {

        if(mp->Map_Tilemap[ipy*mp->Map_Width         + ipx_add_xo] == 0)
            pos_x += step_x;
        if(mp->Map_Tilemap[ipy_add_yo*mp->Map_Height + ipx] == 0)
            pos_y += step_y;

    }
    else if(BWD)
    {

        if(mp->Map_Tilemap[ipy*mp->Map_Width         + ipx_sub_xo] == 0)
            pos_x -= step_x;
        if(mp->Map_Tilemap[ipy_sub_yo*mp->Map_Height + ipx] == 0)
            pos_y -= step_y;

    }
    else if(LEFT)
    {

        if(mp->Map_Tilemap[sipy*mp->Map_Width + sipx_sub_xo] == 0)
            pos_x -= strafe_x;
        if(mp->Map_Tilemap[sipy_sub_yo*mp->Map_Height + sipx] == 0)
            pos_y -= strafe_y;

    }
    else if(RIGHT)
    {

        if(mp->Map_Tilemap[sipy*mp->Map_Width + sipx_add_xo] == 0)
            pos_x += strafe_x;
        if(mp->Map_Tilemap[sipy_add_yo*mp->Map_Height + sipx] == 0)
            pos_y += strafe_y;
    }

    //Door interaction
    int horz = ipy*mp->Map_Width  + ipx_add_xo;
    int vert = ipy_add_yo*mp->Map_Height + ipx;
    if(mp->Map_Tilemap[horz] == 2 || mp->Map_Tilemap[horz] == 4)
    {
        if(mp->Map_Tilemap[horz] == 4 && door)
        {
            if(game::all_coins)
            {
                escaped = true;
            }
            else
            {
                denied->play();
            }
        }
        if(mp->Map_Tilemap[horz] == 2 && door)
        {
            door_open->play();
            mp->Map_Tilemap[horz] = 0;
            doors[horz] = SDL_GetTicks();
        }
    }
    else if(mp->Map_Tilemap[vert] == 2 || mp->Map_Tilemap[vert] == 4)
    {
        if(mp->Map_Tilemap[vert] == 4 && door)
        {
            if(game::all_coins)
            {
                escaped = true;
            }
            else
            {
                denied->play();
            }
        }
        if(mp->Map_Tilemap[vert] == 2 && door)
        {
            door_open->play();
            mp->Map_Tilemap[vert]  = 0;
            doors[vert] = SDL_GetTicks();
        }
    }
    for(auto it : doors)
    {
        if(time-it.second > 3000)
        {
            mp->Map_Tilemap[it.first] = 2;
        }
    }

    player_rect.x = pos_x;
    player_rect.y = pos_y;
    center_x = pos_x + player_rect.w/2;
    center_y = pos_y + player_rect.h/2;
}

void player::kbd_ctr(SDL_Event event)
{
    if(event.type == SDL_KEYDOWN)
    {
        if(event.key.keysym.sym == SDLK_w)
        {
            if(walking == false)
            {
                footstep->play();
                walking = true;
            }
            FWD = true;
        }
        else if(event.key.keysym.sym == SDLK_s)
        {
            if(walking == false)
            {
                footstep->play();
                walking = true;
            }
            BWD = true;
        }
        else if(event.key.keysym.sym == SDLK_a)
        {
            if(walking == false)
            {
                footstep->play();
                walking = true;
            }
            LEFT = true;
        }
        else if(event.key.keysym.sym == SDLK_d)
        {
            if(walking == true)
            {
                footstep->play();
                walking = true;
            }
            RIGHT = true;
        }
        else if(event.key.keysym.sym == SDLK_e)
        {
            door = true;
        }
    }

    if(event.type == SDL_KEYUP)
    {

        if(event.key.keysym.sym == SDLK_w)
        {
            FWD = false;
            walking = false;
            footstep->pause_music();
        }
        else if(event.key.keysym.sym == SDLK_s)
        {
            BWD = false;
            walking = false;
            footstep->pause_music();
        }
        else if(event.key.keysym.sym == SDLK_a)
        {
            LEFT = false;
            walking = false;
            footstep->pause_music();
        }
        else if(event.key.keysym.sym == SDLK_d)
        {
            RIGHT = false;
            walking = false;
            footstep->pause_music();
        }
        else if(event.key.keysym.sym == SDLK_e)
        {
            door = false;
            walking = false;
            footstep->pause_music();
        }
    }
}

void player::mse_ctr(SDL_Event event)
{
    if(event.type == SDL_MOUSEBUTTONDOWN)
    {
        if(event.button.button == SDL_BUTTON_LEFT)
            left_click = true;
    }
    if(event.type == SDL_MOUSEMOTION)
    {
        int mxo = event.motion.xrel;
        if(mxo < 0)
        {
            player_angle -= MOUSE_SENSITIVIY*(-mxo);
            if(player_angle < 0){player_angle += 2*PI;}
            step_x = cos(player_angle)*speed;
            step_y = sin(player_angle)*speed;
        }
        else if(mxo > 0)
        {
            player_angle += MOUSE_SENSITIVIY*mxo;
            if(player_angle > 2*PI){player_angle -= 2*PI;}
            step_x = cos(player_angle)*speed;
            step_y = sin(player_angle)*speed;
        }
        strafe_angle = player_angle+PI/2;
        if(strafe_angle > 2*PI) strafe_angle -= 2*PI;
        if(strafe_angle < 0)    strafe_angle += 2*PI;
        strafe_x = cos(strafe_angle)*speed;
        strafe_y = sin(strafe_angle)*speed;
    }
}

void player::set_default()
{
    footstep->pause_music();
    walking = false;
    FWD = false; BWD = false; LEFT = false; RIGHT = false;
}
