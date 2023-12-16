#pragma once

#include <SDL2\SDL.h>
#include "constants_and_data_types.hpp"
#include "grid.hpp"

class Ship_sheet{
    public:
        Ship_sheet(void);
        ~Ship_sheet(void);
        void set_spritesheet(SDL_Texture *texture);
        void set_position(ship_type s, int x, int y);
        void set_current_position(ship_type s);
        void set_orientation(ship_type s, ship_orientation orientation);
        void draw(SDL_Renderer *renderer, ship_type s);
        ship_type index_to_type(int i);
    
    private:
        ship ship_list[TOTAL_SHIPS];
        SDL_Texture *s_texture;
};