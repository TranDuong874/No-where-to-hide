#ifndef sprites_h
#define sprites_h

#include "player.h"
#include <SDL.h>
#include <bits/stdc++.h>

#include "game.h"

using namespace std;

class sprite
{
protected:
    float plane_x, plane_y;
    float screen_x, screen_y;
    float dx, dy;
    float width3D, height3D;

    SDL_Rect        rect2D;
    SDL_Rect        rect3D;
    SDL_Texture*    Sprite_Texture;

    //Player's information for mapping view
    float center_angle;
    float sprite_angle_1;
    float sprite_angle_2;
    float sprite_angle_3;
    float sprite_angle_4;

    float player_sprite_dst;
    float player_angle;
    float player_x, player_y;
    player* p;

public:
    sprite(){};
    ~sprite(){}
    sprite(float pos_x, float pos_y, float width,
           float height, player* &p, string path);
    void update();
    void render(vector<render_info> &render_list);
    void render_2D      (SDL_Renderer* &renderer);
    bool line_of_sight  (float angle);
    bool player_collide (player* &p);
    SDL_Texture* Load_SpriteTexture(string path);
    void sprite_mapping();



};
#endif // sprites_h


