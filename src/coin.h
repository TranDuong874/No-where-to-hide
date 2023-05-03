#ifndef coin_h
#define coin_h

#include "sprite.h"

class coin : public sprite
{
public:
    coin(float x, float y, float w, float h, player* &p, string path):sprite(x,y,w,h,p,path)
    {
        this->rect2D.w = 32;
        this->rect2D.h = 32;
    };
    coin(){};
    void update(player* &p)
    {
        sprite::update();
        if(player_collide(p))
        {
            state = false;
        }
    }
    bool get_state(){return state;}
private:
    bool state = true;
};
#endif // coin_h


