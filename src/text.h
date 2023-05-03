#ifndef text_h
#define text_h

#include <SDL_ttf.h>
#include <cstring>
#include <string>
using namespace std;

#include "game.h"

class text
{
public:
    //font, size, position, color, text;
    text(){};
    ~text(){};
    text(int font_size, int pos_x, int pos_y, SDL_Color color, string path, string written_text)
    {
        this->font_size = font_size;
        text_rect.x = pos_x;
        text_rect.y = pos_y;
        text_rect.w = 0;
        text_rect.h = 0;
        text_color = color;
        this->written_text = written_text;
        load_text(path);
    }

    void render(SDL_Renderer* &renderer)
    {
        SDL_RenderCopy(renderer,textTexture,NULL,&text_rect);
    }

    SDL_Texture* load_text(string path)
    {

        font = TTF_OpenFont(path.c_str(), font_size);
        SDL_Surface* textureSurface = TTF_RenderText_Blended(font, written_text.c_str(), text_color);
        textTexture                 = SDL_CreateTextureFromSurface(game::renderer,textureSurface);
        SDL_QueryTexture(textTexture,NULL,NULL,&text_rect.w,&text_rect.h);
        SDL_FreeSurface(textureSurface);
    }

    void set_color(SDL_Color color)
    {
        text_color = color;
        SDL_Surface* textureSurface = TTF_RenderText_Blended(font, written_text.c_str(), text_color);
        textTexture                 = SDL_CreateTextureFromSurface(game::renderer,textureSurface);
        SDL_QueryTexture(textTexture,NULL,NULL,&text_rect.w,&text_rect.h);
        SDL_FreeSurface(textureSurface);
    }

    void set_text(string edited_text)
    {
        written_text = edited_text;
        SDL_Surface* textureSurface = TTF_RenderText_Blended(font, edited_text.c_str(), text_color);
        textTexture                 = SDL_CreateTextureFromSurface(game::renderer,textureSurface);
        SDL_QueryTexture(textTexture,NULL,NULL,&text_rect.w,&text_rect.h);
        SDL_FreeSurface(textureSurface);
    }

    bool hover(int x, int y)
    {
        if(x < text_rect.x)               return false;
        if(x > text_rect.x +text_rect.w)  return false;
        if(y < text_rect.y)               return false;
        if(y > text_rect.y + text_rect.h) return false;
        return true;
    }

    SDL_Rect rect()
    {
        return text_rect;
    }
    void set_x(int x)
    {
        text_rect.x = x;
    }
    void set_y(int y)
    {
        text_rect.y = y;
    }
    void set_size(int w, int h)
    {
        text_rect.w *= w;
        text_rect.h *= h;
    }
    string get_text(){return written_text;}
    SDL_Color    text_color = {0,0,0,255};
private:
    int          font_size;
    TTF_Font*    font;
    SDL_Texture* textTexture;
    SDL_Rect     text_rect;
    string written_text;
};
#endif // text_h


