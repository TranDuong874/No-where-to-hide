#include "animated_sprite.h"
#include "game.h"

void asprites::set_dst(int x, int y, int w, int h)
{
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
}

asprites::asprites(string path)
{
    Load_SpriteSheet(path);
}
void asprites::add(int total_col, int total_line, int selected_line, int frames_count)
{
    SDL_Rect src = {0,0,0,0};
    vector<SDL_Rect> tmp;
    SDL_QueryTexture(sprite_sheet,NULL,NULL,&src.w, &src.h);
    src.h /= total_line;
    src.w /= total_col;
    src.y = selected_line*src.h;
    for(int i = 0; i < frames_count; i++)
    {
        src.x += i*src.w;
        tmp.push_back(src);
    }
    clips.push_back(tmp);
}

bool asprites::play_animation(int num)
{

    SDL_RenderCopy(game::renderer,sprite_sheet, &clips[num][frame_cnt], &dst);
    SDL_Delay(1000/120);
    time_per_frame++;
    if(time_per_frame > 5)
    {
        time_per_frame = 0;
        frame_cnt++;
    }
    if(frame_cnt >= clips[num].size()-1)
    {
        frame_cnt = 0;
        return true;
    }
}

void asprites::set_frame_cnt(int n)
{
    frame_cnt = n;
}

void asprites::Load_SpriteSheet(string path)
{
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    SDL_SetColorKey(loadedSurface,SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255,0,255));
    if(loadedSurface == nullptr)
    {
        cout << "Unable to load Animation Surface " << path << " " << SDL_GetError() << endl;
    }
    else
    {
        sprite_sheet = SDL_CreateTextureFromSurface(game::renderer, loadedSurface);
        if(sprite_sheet == nullptr)
        {
            cout << "Unable to load Animation Texture " << path << " " <<SDL_GetError << endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
}
