#include "sprite.h"
#include <SDL.h>
#include <math.h>
#include "game.h"
#include "player.h"

#define D 0.0174533
#define PI 3.14159265359

sprite::sprite(float pos_x, float pos_y, float width3D,
               float height3D, player* &p, string path)
{
    //Init2D
    plane_x  = pos_x;
    plane_y  = pos_y;
    rect2D.x = plane_x;
    rect2D.y = plane_y;
    rect2D.w = 20;
    rect2D.h = 20;

    //Init3D
    this->width3D = width3D;
    this->height3D = height3D;
    screen_x = 0;
    screen_y = 720/2-100;
    rect3D.x = screen_x;
    rect3D.y = screen_y;
    rect3D.w = width3D;
    rect3D.h = height3D;

    //Get player's info
    this->p = p;
    player_angle = p->get_player_angle();
    player_x     = p->get_center_x();
    player_y     = p->get_center_y();

    Sprite_Texture = Load_SpriteTexture(path);
}

void sprite::sprite_mapping()
{
    const float FOV = PI / 3; // 60 degree field of view
    const float MIN_ANGLE = player_angle - FOV / 2;
    const float MAX_ANGLE = player_angle + FOV / 2;

    float dx, dy;
    float SCALE = 1;
    float delta_angle;
    dx = plane_x - player_x;
    dy = plane_y - player_y;
    player_sprite_dst = sqrt(dx*dx+dy*dy);
    center_angle   = atan2(dy+rect2D.h/2,dx+rect2D.w/2);
    sprite_angle_1 = atan2(dy,dx);
    sprite_angle_2 = atan2(dy,dx+rect2D.w);
    sprite_angle_3 = atan2(dy+rect2D.h,dx);
    sprite_angle_4 = atan2(dy+rect2D.h,dx+rect2D.w);

    while(center_angle < MIN_ANGLE){
        center_angle += 2 * PI;
    }
    while(center_angle >= MAX_ANGLE){
        center_angle -= 2 * PI;
    }

    while(sprite_angle_1 < MIN_ANGLE){
        sprite_angle_1 += 2 * PI;
    }
    while(sprite_angle_1 >= MAX_ANGLE){
        sprite_angle_1 -= 2 * PI;
    }

    if(sprite_angle_2 < 2*PI) sprite_angle_2 += 2*PI;
    if(sprite_angle_2 > 2*PI) sprite_angle_2 -= 2*PI;

    if(sprite_angle_3 < 2*PI) sprite_angle_3 += 2*PI;
    if(sprite_angle_3 > 2*PI) sprite_angle_3 -= 2*PI;

    if(sprite_angle_4 < 2*PI) sprite_angle_4 += 2*PI;
    if(sprite_angle_4 > 2*PI) sprite_angle_4 -= 2*PI;

    SCALE = 50/player_sprite_dst;
    //screen_x = 18*(sprite_angle-player_angle)/D+1024/2;
    screen_x = 20*(center_angle-player_angle)/D+1280/2;
    rect3D.x = screen_x;
    rect3D.w *= SCALE;
    rect3D.h *= SCALE;
    rect3D.y = (768/2-rect3D.h/2);

    //cout << screen_x << "..." << endl;
}


void sprite::update()
{
    plane_x = rect2D.x;
    plane_y = rect2D.y;
    rect3D.w = width3D;
    rect3D.h = height3D;
    sprite_mapping();
    player_x = p->get_center_x();
    player_y = p->get_center_y();
    player_angle = p->get_player_angle();
}
//The jump between 0 and 2PI is causing error
void sprite::render(vector<render_info> &render_list)
{
    if(line_of_sight(sprite_angle_1) || line_of_sight(sprite_angle_2)
       || line_of_sight(sprite_angle_3) || line_of_sight(sprite_angle_4))
    {
        //cout << rect3D.x << " ";
        //SDL_RenderCopy(game::renderer,Sprite_Texture,NULL,&rect3D);
        //cout << "rendered...";
        render_info tmp;
        tmp.distance = player_sprite_dst;
        tmp.dst = rect3D;
        tmp.texture = Sprite_Texture;
        SDL_Rect tmp_rect;
        tmp_rect.x = 0;
        tmp_rect.y = 0;
        SDL_QueryTexture(Sprite_Texture,NULL,NULL,&tmp_rect.w,&tmp_rect.h);
        tmp.src = tmp_rect;
        render_list.push_back(tmp);
    }
    //SDL_SetRenderDrawColor(game::renderer,255,0,0,255);
    //SDL_RenderFillRect(game::renderer,&rect2D);
    //SDL_RenderDrawRect(game::renderer,&rect2D);
}

void sprite::render_2D(SDL_Renderer* &renderer)
{
    SDL_SetRenderDrawColor(game::renderer,255,2,0,255);
    SDL_RenderFillRect(game::renderer,&rect2D);
    SDL_RenderDrawRect(game::renderer,&rect2D);
}

bool sprite::line_of_sight(float angle)
{
    if(player_angle >= PI/6 && player_angle <= 2*PI-PI/6)
    {
        if(angle >= player_angle-PI/6 &&
           angle <= player_angle+PI/6)
        {
            //cout << "LOOKING 1 " << player_angle << endl;
            return true;
        }
    }
    else if(player_angle >= 2*PI-PI/6 || player_angle <= PI/6)
    {
        if(player_angle > PI)
        {
            //Left
            if((angle >= player_angle-PI/6 && angle <= 2*PI)
            || /*right*/(angle >= 0 && angle <= PI/6-(2*PI-player_angle))) //PI/6 = half of FOV
            {
                //cout << "LOOKING 2 "<< player_angle << endl;
                return true;
            }
        }
        else if(player_angle <= PI)
        {
            if((angle <= player_angle+PI/6 && angle >= 0)
               ||(angle <= 2*PI && angle >= 2*PI-PI/6+player_angle))
            {

                //cout << "LOKING 3 " << player_angle  << endl;
                return true;
            }
        }
    }
    return false;
}

bool sprite::player_collide(player* &p)
{
    SDL_Rect tmp = p->rect();
    double left_a, left_b;
    double right_a, right_b;
    double top_a, top_b;
    double bot_a, bot_b;

    left_a = rect2D.x;
    right_a = rect2D.x + rect2D.w;
    top_a = rect2D.y;
    bot_a = rect2D.y + rect2D.h;

    left_b = tmp.x;
    right_b = tmp.x + tmp.w;
    top_b = tmp.y;
    bot_b = tmp.y + tmp.h;

    if(bot_a <= top_b)
        return false;
    if(top_a >= bot_b)
        return false;
    if(right_a <= left_b)
        return false;
    if(left_a >= right_b)
        return false;

    return true;
}



SDL_Texture* sprite::Load_SpriteTexture(string path)
{
    SDL_Surface* loaded_Surface = IMG_Load(path.c_str());
    SDL_Texture* loaded_Texture = SDL_CreateTextureFromSurface(game::renderer, loaded_Surface);
    if(loaded_Surface == nullptr)
    {
        cout << "Unable to load map surface" << endl;
    }
    if(loaded_Texture == nullptr)
        cout << "Unable to load map texture" << endl;
    return loaded_Texture;
}


