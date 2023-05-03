#include <SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>

#include "game.h"
#include "map.h"

using namespace std;

Map::Map(string map_name)
{
    Map_LoadMap(map_name);
}

void Map::Map_LoadMap(string map_name)
{
    Map_ImportTexture();

    mapReader.open(map_name);
    mapReader >> Map_Width;
    mapReader >> Map_Height;
    Map_Size = Map_Width*Map_Height;

    Map_TileSet = new SDL_Rect [number_of_Tilesets];
    Map_Tilemap = new int      [Map_Size];
    Map_DstTile = new SDL_Rect [Map_Size];

    for(int i = 0; i < number_of_Tilesets; i++)
    {
        SDL_Rect tmp = {i*tileSize,0,tileSize,tileSize};
        Map_TileSet[i] = tmp;
    }
    for(int i = 0; i < Map_Size; i++)
    {
        int pos;
        mapReader >> Map_Tilemap[i];
    }
    int k = 0;
    for(int i = 0; i < Map_Height; i++)
    {
        for(int j = 0; j < Map_Width; j++)
        {
            //Minimap
            SDL_Rect tmp = {tileSize*j,tileSize*i,tileSize,tileSize};
            Map_DstTile[k] = tmp;
            k++;
        }
    }
}

void Map::Map_Render2D()
{
    for(int i = 0; i < Map_Size; i++)
    {
        Map_SrcTile = Map_TileSet[Map_Tilemap[i]];
        SDL_RenderCopy(game::renderer, Map_TileTexture, &Map_SrcTile, &Map_DstTile[i]);

    }
}

//3D
void Map::Map_WallColor(int wall_index, float shade)
{
    switch(wall_index)
    {
    case 1:
        SDL_SetRenderDrawColor(game::renderer,0,255*shade,0,0);
        break;
    case 2:
        SDL_SetRenderDrawColor(game::renderer,0,255*shade,0,255);
        break;
    case 3:
        SDL_SetRenderDrawColor(game::renderer,255*shade,255*shade,0,255);
        break;
    case 4:
        SDL_SetRenderDrawColor(game::renderer,0,255*shade,255*shade,255);
        break;
    }
}

SDL_Texture* Map::Map_WallTexture (int wall_index, float shade)
{
    return Map_WallTexture_list[wall_index];
}
SDL_Texture* Map::Map_LoadTexture(string path)
{
    SDL_Surface* loaded_Surface = IMG_Load(path.c_str());
    SDL_Texture* loaded_Texture = SDL_CreateTextureFromSurface(game::renderer, loaded_Surface);
    if(loaded_Surface == nullptr)
    {
        cout << "Unable to load map surface" << endl;
    }
    if(loaded_Texture == nullptr)
        cout << "Unable to load map texture" << endl;
    return loaded_Texture;
}






