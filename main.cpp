#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <SDL2\SDL_mixer.h> 
#include <stdio.h>
#include "cursor.cpp"
#include "game.cpp"
#include "grid.cpp"
#include "ship.cpp"

int main(int argc, char *argv[]){
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    Game battleship_game;
    battleship_game.init();
    return 0;
}
