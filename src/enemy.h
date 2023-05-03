#ifndef enemy_h
#define enemy_h

#include "sprite.h"
#include "player.h"

class player;
class Map;

class enemy : public sprite
{
private:
    float speed       = 2;
    SDL_Rect hit_zone = {1280/2-25,0,50,768};
    Uint32 start_time = SDL_GetTicks();
    Uint32 time       = 0;
    bool in_sight      = false;
    bool sound_playing = false;
    bool freeze        = false;
    SDL_Texture* jumpscare = Load_SpriteTexture("Resources/entities/jumpscare.png");
    game_audio* sound = new game_audio("Resources/audio/hehe.wav",1,80);
public:
    enemy(){};
    ~enemy(){};
    enemy(float x, float y, float w, float h, player* &p, string path):sprite(x,y,w,h,p,path)
    {
        this->rect2D.w = 16;
        this->rect2D.h = 16;
    }
    bool get_state(){return freeze;}
    void update(player* &p, Map* &mp);
    void render2D(SDL_Renderer* &renderer);
    bool collision_check(SDL_Rect a, SDL_Rect b);
    bool check_in_sight(){return in_sight;}
    void set_speed(int speed){this->speed = speed;}
    bool in_hit_zone()
    {
        if(collision_check(rect3D,hit_zone)) return true;
        else return false;
    }

};
#endif

