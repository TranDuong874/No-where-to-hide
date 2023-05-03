
#include <math.h>
#include <SDL.h>

#include "game.h"
#include "raycaster.h"

#define PI 3.14159265359
#define PI2 PI/2
#define PI3 3*PI/2
#define D 0.0174533


void raycaster::get_player(player* &p)
{
    this->p = p;
    player_angle = p->get_player_angle();
    center_x     = p->get_center_x();
    center_y     = p->get_center_y();
}

void raycaster::get_map(Map* &mp)
{
    this->mp     = mp;
}
void raycaster::cast_rays(vector<render_info> &render_list)
{
    player_angle = p->get_player_angle();
    center_x     = p->get_center_x();
    center_y     = p->get_center_y();
    ray_angle = player_angle-30*D;
    float shade = 0;

    if     (ray_angle > 2*PI) ray_angle -= 2*PI;
    else if(ray_angle < 0)    ray_angle += 2*PI;

    for(int r = 0; r < 1280; r++)
    {
        check_horizontal();
        check_vertical();
        SDL_SetRenderDrawColor(game::renderer,255,0,0,255);
        ray_angle += D*0.046875; //FOV = 60, FOV/1280 rays = 0.046875
        if     (ray_angle > 2*PI)   ray_angle -= 2*PI;
        else if(ray_angle < 0) ray_angle += 2*PI;

        if(v_distance < h_distance)
        {
            wall_distance = v_distance;
            ray_x = vx;
            ray_y = vy;
            map_point = vert_mp;
            texture_offset = texture_offset_v;
            shade = 0.5;
        }
        else
        {
            wall_distance = h_distance;
            ray_x = hx;
            ray_y = hy;
            map_point = horz_mp;
            texture_offset = texture_offset_h;
            shade = 1;
        }
        //SDL_RenderDrawLineF(game::renderer,center_x,center_y,ray_x,ray_y);
        texture_mapping(r, render_list, shade);
    }
}

void raycaster::texture_mapping(int r, vector<render_info> &render_list, float shade)
{
    float cam_angle = player_angle-ray_angle;
    if(cam_angle < 0)    cam_angle += 2*PI;
    if(cam_angle > 2*PI) cam_angle -= 2*PI;

    wall_distance *= cos(cam_angle);
    if(wall_distance < 1) wall_distance = 1;

    const int actual_wall_height = 768;
    const int player_view_height = 53; //Some how this changes the FOV

    float wall_height = actual_wall_height/wall_distance*player_view_height;
    float wall_offset = (actual_wall_height-wall_height)/2;

    //Draw floors
    SDL_Rect     wall_strip = {r*1,wall_offset+wall_height,1,720};
    SDL_SetRenderDrawColor(game::renderer,112, 96, 77,255);
    SDL_RenderFillRect(game::renderer,&wall_strip);
    SDL_RenderDrawRect(game::renderer,&wall_strip);

    //Wall texture mapping
    wall_strip = {r*1,wall_offset,1,wall_height};
    SDL_Texture* wall_txt   = mp->Map_WallTexture(mp->Map_Tilemap[map_point],1.0);

    SDL_Rect src_rect;
    src_rect.w = 1;
    src_rect.h = wall_height;
    src_rect.y = 0;
    src_rect.x = texture_offset;


    SDL_RenderCopy(game::renderer,wall_txt,&src_rect,&wall_strip);
    render_info tmp;
    tmp.distance = wall_distance;
    tmp.src      = src_rect;
    tmp.dst = wall_strip;
    tmp.texture = wall_txt;

    render_list.push_back(tmp);
    SDL_RenderDrawRect(game::renderer, &wall_strip);
}



void raycaster::check_horizontal()
{
    float aTan = -1/tan(ray_angle);
            dof = 0;
            if(ray_angle > PI)
            {
                ray_y        = (((int)center_y/mp->tileSize)*mp->tileSize)-0.0001;
                ray_x        = center_x + (center_y-ray_y)*aTan;
                ray_offset_y = -mp->tileSize;
                ray_offset_x = -ray_offset_y*aTan;


            }
            else if(ray_angle < PI)
            {
                ray_y        = (((int)center_y/mp->tileSize)*mp->tileSize)+mp->tileSize;
                ray_x        = center_x + (center_y-ray_y)*aTan;
                ray_offset_y = mp->tileSize;
                ray_offset_x = -ray_offset_y*aTan;
            }
            else if(ray_angle == 0 || ray_angle == PI)
            {
                ray_x = center_x;
                ray_y = center_y;
                dof  = max(mp->Map_Width,mp->Map_Height);
            }

            while(dof < max(mp->Map_Width,mp->Map_Height))
            {
                hx = ray_x; hy = ray_y;
                map_x      = (int)ray_x/mp->tileSize;
                map_y      = (int)ray_y/mp->tileSize;
                map_point  = map_y*mp->Map_Width+map_x;

                h_distance = sqrt((ray_x-center_x)*(ray_x-center_x)
                                +(ray_y-center_y)*(ray_y-center_y));

                if(map_point > 0 && map_point < mp->Map_Size &&
                   mp->Map_Tilemap[map_point] > 0)
                {
                    texture_offset_h = (int)ray_x%mp->tileSize;
                    horz_mp = map_point;
                    break;
                }
                else
                {
                    ray_x += ray_offset_x;
                    ray_y += ray_offset_y;
                    dof++;
                }
            }
}

void raycaster::check_vertical()
{
    dof = 0;
    float nTan = -tan(ray_angle);
    if(ray_angle > PI/2 && ray_angle < 3*PI/2)
    {
        ray_x        = (((int)center_x/mp->tileSize)*mp->tileSize)-0.0001;
        ray_y        = center_y + (center_x-ray_x)*nTan;
        ray_offset_x = -mp->tileSize;
        ray_offset_y = -ray_offset_x*nTan;

    }
    else if(ray_angle < PI/2 || ray_angle > 3*PI/2)
    {
        ray_x        = (((int)center_x/mp->tileSize)*mp->tileSize)+mp->tileSize;
        ray_y        = center_y + (center_x-ray_x)*nTan;
        ray_offset_x = mp->tileSize;
        ray_offset_y = -ray_offset_x*nTan;
    }
    else if(ray_angle == 0 || ray_angle == PI)
    {
        ray_x = center_x;
        ray_y = center_y;
        dof = max(mp->Map_Width,mp->Map_Height);
    }

    while(dof < max(mp->Map_Width,mp->Map_Height))
    {
        vx         = ray_x; vy = ray_y;
        map_x      = (int)ray_x/mp->tileSize;
        map_y      = (int)ray_y/mp->tileSize;
        map_point  = map_y*mp->Map_Width+map_x;

        v_distance = sqrt((ray_x-center_x)*(ray_x-center_x)
                          +(ray_y-center_y)*(ray_y-center_y));
        if(map_point > 0 && map_point < mp->Map_Size && mp->Map_Tilemap[map_point] > 0)
        {
            texture_offset_v = (int)ray_y%mp->tileSize;
            vert_mp = map_point;
            break;
        }
        else
        {
            ray_x += ray_offset_x;
            ray_y += ray_offset_y;
            dof++;
        }
    }
}

