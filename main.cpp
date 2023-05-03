#include "src/game.h"

int main(int argc, char* argv[])
{
    game* GAME = new game;
    GAME->init();
    while(GAME->running)
    {
        GAME->start_screen();
        GAME->game_loop();
    }
    return 0;
}
