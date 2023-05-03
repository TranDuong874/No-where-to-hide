#include "game.h"
#include "text.h"
#include <fstream>

void game::win_screen()
{
    bool active = true;
    PLAYER->set_default();
    SDL_SetRelativeMouseMode(SDL_FALSE);
    SDL_Color white = {255,255,255,255};
    SDL_Color red   = {255,0,0,0};
    text restart   (50,50,SCREEN_HEIGHT-50,white,font,"restart");
    text main_menu (50,50,SCREEN_HEIGHT-50,white,font,"main menu");
    main_menu.set_x(SCREEN_WIDTH-main_menu.rect().w-50);

    int fin_sec          = second_passed;
    text fin_time = *time_clock;
    fin_time.set_x(SCREEN_WIDTH/2-fin_time.rect().w/2);
    int x, y;
    bool clicked = false;
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    ///////////////////////////////////////////
    string old_rect_text;
    ifstream read; read.open("record.txt");
    int old_record;
    read >> old_record;
    read >> old_rect_text;
    read.close();
    int new_rec = 0; bool achieved_nr = false;
    if(fin_sec < old_record)
    {
        new_rec = fin_sec;
        achieved_nr = true;
        fin_time.set_size(3,3);
        fin_time.set_y(300);
        fin_time.set_x(SCREEN_WIDTH/2-fin_time.rect().w/2);
        ofstream write;
        write.open("record.txt");
        write << new_rec;
        write << endl;
        write << fin_time.get_text();
        write.close();
    }

    text best_record    (100,-1,200,white,font,"best record");
    best_record.set_x(SCREEN_WIDTH/2-best_record.rect().w/2);
    text new_record     (100,-1,200,white,font,"new record");
    new_record.set_x(SCREEN_WIDTH/2-new_record.rect().w/2);
    text best_rec_val   (100,-1,300,white,font, old_rect_text);
    best_rec_val.set_x(SCREEN_WIDTH/2-best_rec_val.rect().w/2);
    //new rec is fin sec
    ///////////////////////////////////////////
    while(active)
    {
        SDL_RenderClear     (renderer);
        restart.render      (renderer);
        main_menu.render    (renderer);

        if(achieved_nr)
        {
            new_record.render(renderer);
            fin_time.render(renderer);
        }
        else
        {
            fin_time.render(renderer);
            best_record.render(renderer);
            best_rec_val.render(renderer);
        }

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
