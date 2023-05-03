#ifndef asprites_h
#define asprites_h
#include <SDL.h>
#include <vector>
#include <string>
#include <SDL_image.h>

#include "game.h"

using namespace std;

class asprites
{
private:
    SDL_Texture*             sprite_sheet;
    vector<vector<SDL_Rect>> clips;
    int                      frame_cnt = 0;
    int                      time_per_frame = 0;
    SDL_Rect                 dst;
public:
    asprites(){};
    void set_dst(int x, int y, int w, int h);
    asprites(string path);
    void add(int total_col, int total_line, int selected_line, int frames_count);
    bool play_animation(int num);
    void set_frame_cnt(int n);

    void Load_SpriteSheet(string path);

};
#endif // asprites_h
