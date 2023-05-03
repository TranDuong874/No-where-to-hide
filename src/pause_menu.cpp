#include "game.h"
#include "text.h"

void game::pause_menu()
{
    PLAYER->set_default();
    bool active = true;
    SDL_Color white =   {255,255,255,255};
    SDL_Color red   =   {255,0,0,255};
    text game_paused    (60,0,30,white,"Resources/fonts/BRADHITC.TTF","GAME PAUSED");
    text resume         (50,SCREEN_WIDTH,200,white,"Resources/fonts/BRADHITC.TTF","resume");
    text option         (50,SCREEN_WIDTH,200+80,white,"Resources/fonts/BRADHITC.TTF","option");
    text quit           (50,SCREEN_WIDTH,200+80+80,white,"Resources/fonts/BRADHITC.TTF","main menu");

    game_paused.set_x   (SCREEN_WIDTH/2-game_paused.rect().w/2);
    resume.set_x        (SCREEN_WIDTH/2-resume.rect().w/2);
    option.set_x        (SCREEN_WIDTH/2-option.rect().w/2);
    quit.set_x          (SCREEN_WIDTH/2-quit.rect().w/2);

    SDL_SetRelativeMouseMode(SDL_FALSE);
    pause_time  = SDL_GetTicks();
    while(active)
    {
        SDL_RenderClear(renderer);
        //Hover on button
        int x,y;
        bool clicked = false;
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        //Render
        game_paused.render  (renderer);
        option.render       (renderer);
        quit.render         (renderer);
        resume.render       (renderer);
        SDL_RenderPresent   (renderer);

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

        if(resume.hover(x,y))
        {
            resume.set_color(red);
            if(clicked)
            {
                start_time += SDL_GetTicks()-pause_time;
                pause = !pause;
                active = false;
                SDL_SetRelativeMouseMode(SDL_TRUE);
            }

        }
        else
        {
            resume.set_color(white);
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
                warning();
                if(playing == false)
                {
                    active = false;
                }
            }
        }
        else
        {
            quit.set_color(white);
        }
    }
}
