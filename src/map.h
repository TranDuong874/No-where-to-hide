#ifndef map_h
#define map_h

#include <SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>

using namespace std;

struct Map
{
//Variables
    int Map_Width, Map_Height;
    int Map_Size;

    int*         Map_Tilemap;
    SDL_Rect*    Map_DstTile;

    SDL_Rect*    Map_TileSet;
    SDL_Texture* Map_BackGround;
    SDL_Texture* Map_TileTexture;
    SDL_Texture* Map_WallTexture_list[11];
    SDL_Rect     Map_SrcTile;
    const int    tileSize = 32;
    const int    number_of_Tilesets = 11;

    ifstream     mapReader;

//Functions
    Map(){};
    Map                          (string map_name);
    ~Map(){};

    void Map_LoadMap             (string map_name);
    void Map_Render2D();
    void Map_WallColor           (int wall_index,float shade);
    SDL_Texture* Map_WallTexture (int wall_index, float shade);
    SDL_Texture* Map_LoadTexture (string path);

    void Map_ImportTexture()
    {
    Map_TileTexture         = Map_LoadTexture("Resources/map/tileSet.bmp");

    Map_WallTexture_list[1] = Map_LoadTexture("Resources/wall/brickwall.png");
    Map_WallTexture_list[2] = Map_LoadTexture("Resources/wall/door.png");
    Map_WallTexture_list[3] = Map_LoadTexture("Resources/wall/crate.png");
    Map_WallTexture_list[4] = Map_LoadTexture("Resources/wall/exit.png");
    Map_WallTexture_list[5] = Map_LoadTexture("Resources/wall/cow.png");
    Map_WallTexture_list[6] = Map_LoadTexture("Resources/wall/rock.png");
    Map_WallTexture_list[7] = Map_LoadTexture("Resources/wall/stiv.png");
    Map_WallTexture_list[8] = Map_LoadTexture("Resources/wall/stone.png");
    Map_WallTexture_list[9] = Map_LoadTexture("Resources/wall/who.png");
    Map_WallTexture_list[10] = Map_LoadTexture("Resources/wall/white.png");
    }

};

#endif // map_h

