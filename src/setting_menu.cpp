#include "game.h"
#include "text.h"
#include "button.h"
#include "config.h"

void game::setting_menu()
{
    SDL_SetRelativeMouseMode(SDL_FALSE);
    bool active     = true;
    bool clicked    = false;
    SDL_Color white = {255,255,255,255};
    SDL_Color red   = {255,0,0,0};

    text setting_title  (60,0,30,white,font,"Settings");
    setting_title.set_x (SCREEN_WIDTH/2 - setting_title.rect().w/2);
    text volume         (50,50,200,white,font,"volume");
    text mouse_sen      (50,50,200+80,white,font,"mouse sensitivity");
    text fullscreen     (50,50,200+80+80,white,font,"fullscreen");
    text enable_fsc     (50,SCREEN_WIDTH/2,200+80+80,white,font,"enabled");
    text back           (50,1280-200,200+80*5,white,font,"back");

    SDL_Rect volume_bar = {SCREEN_WIDTH/2,200 + 25,300,10};
    button volume_rect(SCREEN_WIDTH/2+3*VOLUME,200+20,20,20);
    text vol_num(50,SCREEN_WIDTH/2 + 300 + 30,200,white,font,to_string((int)VOLUME));

    button sen_rect(SCREEN_WIDTH/2+15*(MOUSE_SENSITIVIY*10000),200+20+80,20,20);
    SDL_Rect sen_bar = {SCREEN_WIDTH/2,200 + 25 + 80,300,10};
    text sen_num(50,SCREEN_WIDTH/2 + 300 + 30,200 + 80,white,font,to_string(int(MOUSE_SENSITIVIY*10000)));

    SDL_Texture* background = LoadTexture("Resources/overlay/setting.png");
    int knob_min = SCREEN_WIDTH/2;
    int knob_max = SCREEN_WIDTH/2 + 300 - 20;

    while(active)
    {
        SDL_RenderClear(renderer);
        //Hover on button
        int x,y;

        //Render
        SDL_RenderCopy(renderer,background,NULL,NULL);
        setting_title.render(renderer);
        volume.render(renderer);
        back.render(renderer);
        fullscreen.render(renderer);
        mouse_sen.render(renderer);
        enable_fsc.render(renderer);
        SDL_RenderDrawRect(renderer,&volume_bar);
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        volume_rect.render(renderer);
        SDL_RenderDrawRect(renderer,&sen_bar);
        sen_rect.render(renderer);
        sen_num.render(renderer);
        vol_num.render(renderer);
        SDL_RenderPresent(renderer);


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

        if(back.hover(x,y))
        {
            back.set_color(red);
            if(clicked)
            {
                active = false;
            }
        }
        else
        {
            back.set_color(white);
        }

        if(sen_rect.hover())
        {
            sen_rect.set_color(red);
            int max_bar = 300;
            int min_bar = 0;
            sen_rect.set_color(red);
            while(clicked)
            {
                SDL_PollEvent(&event);
                //cout << "Holidng";
                if(event.type == SDL_MOUSEBUTTONUP)
                {
                    //cout << "Released";
                    clicked = false;
                    break;
                }

                SDL_GetMouseState(&x,&y);
                sen_rect.set_x(x);
                if(sen_rect.rect().x < knob_min) sen_rect.set_x(knob_min);
                if(sen_rect.rect().x > knob_max) sen_rect.set_x(knob_max);
                MOUSE_SENSITIVIY = 0.0001*(x-knob_min)/20; //bar width = 300, 20 sen level, 1 sen = 0.00001 actual sen, 300/20 = 15
                //Display sen = 1 to 20, => display sen = actual sen*1e5
                int displayed_sen = 10000*MOUSE_SENSITIVIY;
                if(MOUSE_SENSITIVIY < 0.0001){   MOUSE_SENSITIVIY = 0.0001; displayed_sen = 0;}
                if(MOUSE_SENSITIVIY > 0.002)  {   MOUSE_SENSITIVIY = 0.002;   displayed_sen = 20;}

                //cout << MOUSE_SENSITIVIY;
                sen_num.set_text(to_string((int)displayed_sen));
                sen_rect.render(renderer);
            }
        }
        else
        {
            sen_rect.set_color(white);
        }

        if(volume_rect.hover())
        {
            int max_bar = 300;
            int min_bar = 0;
            volume_rect.set_color(red);
            while(clicked)
            {
                SDL_PollEvent(&event);
                //cout << "Holidng";
                if(event.type == SDL_MOUSEBUTTONUP)
                {
                    //cout << "Released";
                    clicked = false;
                    break;
                }

                SDL_GetMouseState(&x,&y);
                volume_rect.set_x(x);
                if(volume_rect.rect().x < knob_min) volume_rect.set_x(knob_min);
                if(volume_rect.rect().x > knob_max) volume_rect.set_x(knob_max);
                VOLUME = (x-knob_min)/3; //bar width = 300, 300/100 = 3, 3 pixel = 1 volume unit
                if(VOLUME < 0)   VOLUME = 0;
                if(VOLUME > 100) VOLUME = 100;
                vol_num.set_text(to_string((int)VOLUME));
                volume_rect.render(renderer);
            }
        }
        else
        {
            volume_rect.set_color(white);
        }

        if(enable_fsc.hover(x,y))
        {
            enable_fsc.set_color(red);
            if(clicked)
            {
                FULL_SCREEN = !FULL_SCREEN;
                if(FULL_SCREEN)
                {
                    SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);
                    enable_fsc.set_text("disabled");
                }
                else
                {

                    SDL_SetWindowFullscreen(window,0);
                    SDL_SetWindowSize(window, SCREEN_WIDTH,SCREEN_HEIGHT);
                    enable_fsc.set_text("enabled");
                }
            }
        }
        else
        {
            enable_fsc.set_color(white);
        }
    }
}
