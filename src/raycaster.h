#ifndef raycaster_h
#define raycaster_h

#include <vector>
#include "game.h"

using namespace std;

class player;
class Map;

class raycaster
{
private:
    Map*    mp;
    player* p;
    //Ray casting
    int   map_x, map_y, map_point, dof;
    float ray_x, ray_y, ray_angle,
          ray_offset_x, ray_offset_y;
    float vx, vy,
          hx, hy;
    float v_distance = 1e9,
          h_distance = 1e9,
          wall_distance;
    float player_angle;
    float center_x, center_y;

   //Texture mapping
    int texture_offset_h,
        texture_offset_v,
        texture_offset;
    int vert_mp, horz_mp;

public:
    raycaster(){};
    ~raycaster(){};
    void get_player (player* &p);
    void get_map    (Map* &mp);
    void cast_rays  (vector<render_info> &render_list);
    void draw_wall();
    void check_horizontal();
    void check_vertical();
    void texture_mapping(int ray_index,vector<render_info> &render_list, float shade);

};

#endif
