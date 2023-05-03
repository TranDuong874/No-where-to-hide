#include "enemy.h"
#include "bits/stdc++.h"

#include "game.h"

using namespace std;

void enemy::update(player* &p, Map* &mp)
{
    time = SDL_GetTicks();
    sprite::update();

    //Check in gun sight
    int cnt = 0;
    for(int tile = 0; tile < mp->Map_Size; tile++)
    {
        if(mp->Map_Tilemap[tile] > 0)
        {
            int px = player_x, py = player_y;
            int ex = rect2D.x+rect2D.w/2, ey = rect2D.y+rect2D.h/2;
            cnt += SDL_IntersectRectAndLine(&(mp->Map_DstTile[tile]),&px,&py,&ex,&ey);
        }
    }
    if(cnt == 0)
        in_sight = true;
    else
        in_sight = false;
    /*if(in_sight)
        cout << "in sight";*/

    //Check if shot
    //Other kind of check (but I dont like)
    //get shot_time, shot_time += 5000, if(current_time - shot_time > 0) speed = 2;
    if(speed == 0)
    {
        start_time = SDL_GetTicks();
        freeze = true;
    }
    else
    {
        start_time = time;
    }
    if(time - start_time > 5000)
    {
        speed = 2;
        freeze = false;
    }

    //Sounds
    sound->set_volume(VOLUME*400/player_sprite_dst);
    if(game::second_passed%5==0)
    {
        if(sound_playing == false)
        {
            sound->play();
            cout << "Played" << game::second_passed/60 << ":" << game::second_passed%60 << endl;
            sound_playing = true;
        }
    }
    else if(game::second_passed%5!=0)
    {
        sound_playing = false;
    }

    //Map collision
    int enemy_mpx, enemy_mpy, enemy_mp;
    enemy_mpx = (int)rect2D.x/mp->tileSize;
    enemy_mpy = (int)rect2D.y/mp->tileSize;

    int add_x = (rect2D.x+16*2)/mp->tileSize,
        add_y = (rect2D.y+16*2)/mp->tileSize;
    int sub_x = (rect2D.x-4)/mp->tileSize,
        sub_y = (rect2D.y-4)/mp->tileSize;

    if(rect2D.x > p->get_center_x()
       && mp->Map_Tilemap[enemy_mpy*mp->Map_Width + sub_x] == 0) rect2D.x -= speed;
    if(rect2D.x < p->get_center_x()
       && mp->Map_Tilemap[enemy_mpy*mp->Map_Width + add_x] == 0) rect2D.x += speed;
    if(rect2D.y > p->get_center_y()
       && mp->Map_Tilemap[sub_y*mp->Map_Width + enemy_mpx] == 0) rect2D.y -= speed;
    if(rect2D.y < p->get_center_y()
       && mp->Map_Tilemap[add_y*mp->Map_Width + enemy_mpx] == 0) rect2D.y += speed;
}

bool enemy::collision_check(SDL_Rect a, SDL_Rect b)
{
    //cout << "Colliding";
    double left_a, left_b;
    double right_a, right_b;
    double top_a, top_b;
    double bot_a, bot_b;

    left_a = a.x;
    right_a = a.x + a.w;
    top_a = a.y;
    bot_a = a.y + a.h;

    left_b = b.x;
    right_b = b.x + b.w;
    top_b = b.y;
    bot_b = b.y + b.h;

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

void enemy::render2D(SDL_Renderer* &renderer)
{
    SDL_SetRenderDrawColor(game::renderer,255,0,0,255);
    SDL_RenderFillRect(game::renderer,&rect2D);
    SDL_RenderDrawRect(game::renderer,&rect2D);
}

