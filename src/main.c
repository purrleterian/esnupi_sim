#include "game.h"


int main(int argc, char **argv) {
    bool exit_status = EXIT_FAILURE;
    srand(time(NULL));

    Game *game = NULL;

    if (game_setup(&game)) {
        game_run(game);
        exit_status = EXIT_SUCCESS;
    }


    game_free(&game);
    return exit_status;
}

