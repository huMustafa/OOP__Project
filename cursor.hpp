#pragma once
#include <SDL2\SDL.h>
#include "grid.hpp"
#include "cursor.hpp"
#include "constants_and_data_types.hpp"
#include "Drawable.hpp"

class Cursor : public Drawable{
    public:
        Cursor(Grid *grid_to_use, grid_mode mode_to_use, int w, int h);
        void set_texture(SDL_Texture *texture);
        void set_init_position(void);
        bool tile_is_free(int x, int y);
        bool fits_in_coordinate(int x, int y);
        bool move_up(void);
        bool move_down(void);
        bool move_right(void);
        bool move_left(void);
        void set_ship_position(void);
        ship_type next_ship(void);
        bool toggle_ship_orientation(void);
        void set_screen_position(void);
        void draw(SDL_Renderer *renderer)override;
        void swap_dimensions(void);
        ship_type current_ship_in_use(void);
        tile_state try_bomb_enemy(void);
        void assign_ship_position(void);
    private:
        position c_position;
        int width;
        int height;

        ship_type current_ship;
        ship_orientation current_ship_orientation;   
        grid_mode mode;
        Grid *current_grid;
        SDL_Texture *c_texture;
        SDL_Rect screen_position;
};