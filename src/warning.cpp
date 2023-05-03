#include "game.h"
#include "text.h"

void game::warning()
{
    bool active = true;
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_Color white = {255,255,255,255};
    SDL_Color red   = {255,0,0,255};

    text yes(100,200,300,white,font,"yes");
    text no (100,SCREEN_WIDTH-350,300, white, font, "no");
    text warn(50,0,100,white,font,"WARNING");
    text word(70,0,180,red,font,"THE PROGRESS WILL NOT BE SAVED");
    word.set_x(SCREEN_WIDTH/2-word.rect().w/2);
    warn.set_x(SCREEN_WIDTH/2-warn.rect().w/2);

    while(active)
    {
        SDL_RenderClear(renderer);
        word.render(renderer);
        yes.render(renderer);
        no.render(renderer);
        warn.render(renderer);
        SDL_RenderPresent(renderer);
        int x, y;
        bool clicked = false;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                clicked = true;
            }
            if(event.type = SDL_MOUSEMOTION)
            {
                SDL_GetMouseState(&x,&y);
            }
        }

        SDL_GetMouseState(&x,&y);
        if(yes.hover(x,y))
        {
            yes.set_color(red);
            if(clicked)
            {
                active = false;
                playing = false;
            }
        }
        else
        {
            yes.set_color(white);
        }

        if(no.hover(x,y))
        {
            no.set_color(red);
            if(clicked)
            {
                active = false;
            }
        }
        else
        {
            no.set_color(white);
        }
    }
}
