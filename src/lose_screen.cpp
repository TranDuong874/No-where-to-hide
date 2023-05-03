#include "game.h"
#include "text.h"

void game::lose_screen()
{
    PLAYER->set_default();
    bool active = true;
    SDL_SetRelativeMouseMode(SDL_FALSE);

    SDL_Texture* jumpscare = LoadTexture("Resources/entities/jumpscare.png");
    SDL_Texture* died       = LoadTexture("Resources/entities/died.png");
    game_audio*  scream     = new game_audio("Resources/audio/jc.wav",-1,80);

    bool sound_played = false;

    int start_time    = SDL_GetTicks();
    int running_time      = 0;

    while(running_time - start_time < 4000)
    {
        running_time = SDL_GetTicks();
        if(sound_played == false)
        {
            scream->play();
            sound_played = true;
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,jumpscare,NULL,NULL);
        SDL_RenderPresent(renderer);
        while(SDL_PollEvent(&event)) continue;
    }
    int alpha = 0;
    int alpha_speed = 0;
    start_time = SDL_GetTicks();
    running_time=0;
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    while(running_time-start_time < 4000)
    {
        //7000-5000 = 2000/255 =
        alpha++;
        running_time = SDL_GetTicks();
        SDL_RenderClear(renderer);
        SDL_SetTextureAlphaMod(died,alpha);
        SDL_RenderCopy(renderer,died,NULL,NULL);
        SDL_RenderPresent(renderer);
        while(SDL_PollEvent(&event)) continue;
    }
    SDL_SetTextureAlphaMod(died,255);
    int x, y;
    bool clicked = false;
    SDL_Color white = {255,255,255,255};
    SDL_Color red   = {255,0,0,0};
    text restart   (50,50,SCREEN_HEIGHT-50-50,white,font,"restart");
    text main_menu (50,0,SCREEN_HEIGHT-50-50,white,font,"main menu");
    main_menu.set_x(SCREEN_WIDTH-main_menu.rect().w-50);

    while(active)
    {
        SDL_RenderClear     (renderer);
        SDL_RenderCopy      (renderer,died,NULL,NULL);
        restart.render      (renderer);
        main_menu.render    (renderer);


        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                clicked = true;
            }
            else if(event.type == SDL_MOUSEBUTTONUP)
            {
                clicked = false;
            }
            if(event.type == SDL_MOUSEMOTION)
            {
                SDL_GetMouseState(&x,&y);
            }
        }
        if(restart.hover(x,y))
        {
            restart.set_color(red);
            if(clicked)
            {
                active = false;
                playing = true;
                SDL_SetRelativeMouseMode(SDL_TRUE);
                reset();
                new_game();
            }
        }
        else
        {
            restart.set_color(white);
        }

        if(main_menu.hover(x,y))
        {
            main_menu.set_color(red);
            if(clicked)
            {
                active  = false;
                playing = false;
            }
        }
        else
        {
            main_menu.set_color(white);
        }

        SDL_RenderPresent   (renderer);
    }
}
