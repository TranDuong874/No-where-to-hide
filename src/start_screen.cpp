#include "game.h"
#include "text.h"

void game::start_screen()
{
    SDL_SetRelativeMouseMode(SDL_FALSE);
    bool active = true;
    SDL_Color white = {255,255,255,255};
    SDL_Color red   = {255,0,0,0};
    SDL_Texture* bkg = LoadTexture("Resources/overlay/cover.png");
    text start  (60,60,300,white,font,"start");
    text option (60,60,300+80,white,font,"option");
    text quit   (60,60,300+80+80,white,font,"quit");

    while(active)
    {
        SDL_RenderClear(renderer);
        int x, y;
        bool clicked = false;
        SDL_RenderCopy(renderer,bkg,NULL,NULL);
        option.render(renderer);
        start.render(renderer);
        quit.render(renderer);
        SDL_SetRenderDrawColor(renderer,1,1,1,255);
        SDL_RenderPresent(renderer);

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

        if(start.hover(x,y))
        {
            start.set_color(red);
            playing = true;
            if(clicked) active  = false;
        }
        else
        {
            start.set_color(white);
        }
        if(option.hover(x,y))
        {
            option.set_color(red);
            if(clicked) setting_menu();
        }
        else
        {
            option.set_color(white);
        }
        if(quit.hover(x,y))
        {
            quit.set_color(red);
            if(clicked)
            {
                active = false;
                running = false;
                playing = false;
                SDL_Quit();
                return;
            }
        }
        else
        {
            quit.set_color(white);
        }
    }
}
