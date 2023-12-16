#pragma once
#include <SDL2\SDL.h>
#include "ship.hpp"
#include "constants_and_data_types.hpp"

class Grid{
    public:
        Grid();
        ~Grid(void);
        void set_textures(SDL_Texture *ocean, SDL_Texture *radar, SDL_Texture *tiles);
        void set_enemy(Grid *e);
        void draw_radar(SDL_Renderer *renderer);
        void draw_ocean(SDL_Renderer *renderer);
        void draw_ships(SDL_Renderer *renderer, ship_type ship_to_draw);
        void draw_radar_tiles(SDL_Renderer *renderer);
        void draw_ocean_tiles(SDL_Renderer *renderer);
        tile_state  bomb_tile(int x, int y);
        bool bomb_enemy_tile(int x, int y);
        void init_ships(SDL_Texture *ships_spritesheet);
        void set_ship_position(ship_type ship, int x, int y, ship_orientation orientation);
        void assign_ship_to_tiles(ship_type ship, int x_pos, int y_pos, ship_orientation orientation);
        bool tile_is_bombed(int x, int y);
        bool tile_has_ship(int x, int y);
        bool won(void);

    private:
        Grid *enemy;
        int score;
        tile player_grid [GRID_WIDTH + 1][GRID_HEIGHT + 1];
        Ship_sheet player_ships;
        SDL_Rect radar_position;
        SDL_Rect ocean_position;
        SDL_Texture *radar_texture;
        SDL_Texture *ocean_texture;
        SDL_Texture *tokens_spritesheet;
};