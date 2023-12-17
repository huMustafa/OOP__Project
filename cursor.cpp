#pragma once
#include "cursor.hpp"

// Constructor for Cursor class
Cursor::Cursor(Grid *grid_to_use, grid_mode mode_to_use, int w, int h) {
    current_grid = grid_to_use;
    mode = mode_to_use;
    width = w;
    height = h;
    current_ship = CARRIER;
    current_ship_orientation = UP;
    set_init_position();
}

// Set initial position of the cursor
void Cursor::set_init_position(void) {
    for (int y = 1; y < GRID_HEIGHT; y++) {
        for (int x = 1; x < GRID_WIDTH; x++) {
            if (fits_in_coordinate(x, y)) {
                c_position = {x, y}; 

                // Set screen position based on mode
                if (mode == RADAR_MODE)
                    screen_position = {0, 0, width * TILE_SIZE, height * TILE_SIZE};
                else
                    screen_position = {0, 0, width * TILE_SIZE, height * TILE_SIZE}; 

                // Set ship position based on cursor position
                set_ship_position();
                x = 100;  // Break outer loop
                y = 100;  // Break inner loop
            }
        }
    }
}

// Set texture for rendering
void Cursor::set_texture(SDL_Texture *texture) {
    c_texture = texture;
}

// Check if a tile is free based on the cursor mode
bool Cursor::tile_is_free(int x, int y) {
    if (x < 11 && x > 0 && y < 11 && y > 0) {
        switch (mode) {
            case RADAR_MODE:
                return !current_grid->tile_is_bombed(x, y);

            case OCEAN_MODE:
            default:
                return !current_grid->tile_has_ship(x, y);
        }
    }
    return false;
}

// Check if the cursor fits in a given coordinate
bool Cursor::fits_in_coordinate(int x, int y) {
    for (int horizontal_check = 0; horizontal_check < width; horizontal_check++) {
        if (!tile_is_free(x + horizontal_check, y))
            return false;
    }
    for (int vertical_check = 0; vertical_check < height; vertical_check++) {
        if (!tile_is_free(x, y + vertical_check))
            return false;
    }
    return true;
}

// Move the cursor up
bool Cursor::move_up(void) {
    if (c_position.y > 1) {
        if (mode == OCEAN_MODE) {
            if (fits_in_coordinate(c_position.x, c_position.y - 1)) {
                c_position.y -= 1;
                return true;
            }
        } else {
            c_position.y -= 1;
            return true;
        }
    }
    return false;
}

// Move the cursor down
bool Cursor::move_down(void) {
    if (c_position.y < GRID_HEIGHT) {
        if (mode == OCEAN_MODE) {
            if (fits_in_coordinate(c_position.x, c_position.y + 1)) {
                c_position.y += 1;
                return true;
            }
        } else {
            c_position.y += 1;
            return true;
        }
    }
    return false;
}

// Move the cursor right
bool Cursor::move_right(void) {
    if (c_position.x < GRID_WIDTH) {
        if (mode == OCEAN_MODE) {
            if (fits_in_coordinate(c_position.x + 1, c_position.y)) {
                c_position.x += 1;
                return true;
            }
        } else {
            c_position.x += 1;
            return true;
        }
    }
    return false;
}

// Move the cursor left
bool Cursor::move_left(void) {
    if (c_position.x > 1) {
        if (mode == OCEAN_MODE) {
            if (fits_in_coordinate(c_position.x - 1, c_position.y)) {
                c_position.x -= 1;
                return true;
            }
        } else {
            c_position.x -= 1;
            return true;
        }
    }
    return false;
}

// Switch to the next ship type and set initial position
ship_type Cursor::next_ship(void) {
    current_ship_orientation = UP;
    switch (current_ship) {
        case CARRIER:
            current_ship = BATTLESHIP;
            width = 1;
            height = 4;
            set_init_position();
            break;

        case BATTLESHIP:
            current_ship = CRUISER;
            width = 1;
            height = 3;
            set_init_position();
            break;

        case CRUISER:
            current_ship = SUBMARINE;
            width = 1;
            height = 3;
            set_init_position();
            break;
        
        case SUBMARINE:
            current_ship = DESTROYER;
            width = 1;
            height = 2;
            set_init_position();
            break;
        
        case DESTROYER:
            current_ship = TOTAL_SHIPS;
            break;
    }
    return current_ship;
}

// Toggle ship orientation and adjust position if valid
bool Cursor::toggle_ship_orientation(void) {
    swap_dimensions();
    if (fits_in_coordinate(c_position.x, c_position.y)) {
        switch (current_ship_orientation) {
            case UP:
                current_ship_orientation = RIGHT;
                break;

            case RIGHT:
                current_ship_orientation = DOWN;
                break;

            case DOWN:
                current_ship_orientation = LEFT;
                break;
            
            default:
                current_ship_orientation = UP;
                break;
        }
        return true;
    } else {
        swap_dimensions();
        return false;
    }
}

// Set ship position in the grid
void Cursor::set_ship_position(void) {
    current_grid->set_ship_position(current_ship, c_position.x, c_position.y, current_ship_orientation);
}

// Set screen position based on cursor mode
void Cursor::set_screen_position(void) {
    switch (mode) {
        case RADAR_MODE:
            screen_position.x = TILES_RADAR_INIT_X_POS + ((c_position.x - 1) * TILE_SIZE);
            screen_position.y = TILES_RADAR_INIT_Y_POS + ((c_position.y - 1) * TILE_SIZE);
            break;
        
        case OCEAN_MODE:
        default:
            screen_position.x = TILES_OCEAN_INIT_X_POS + ((c_position.x - 1) * TILE_SIZE);
            screen_position.y = TILES_OCEAN_INIT_Y_POS + ((c_position.y - 1) * TILE_SIZE);
            break;
    }
}

// Render the cursor
void Cursor::draw(SDL_Renderer *renderer) {
    set_screen_position();
    SDL_RenderCopy(renderer, c_texture, &TILE_SPRITES.green, &screen_position);
}

// Swap cursor dimensions
void Cursor::swap_dimensions(void) {
    int temp = width;
    width = height;
    height = temp;
}

// Get the current ship type in use
ship_type Cursor::current_ship_in_use(void) {
    return current_ship;
}

// Try to bomb the enemy and get the tile state
tile_state Cursor::try_bomb_enemy(void) {
    return current_grid->bomb_tile(c_position.x, c_position.y);
}

// Assign ship position to tiles in the grid
void Cursor::assign_ship_position(void) {
    current_grid->assign_ship_to_tiles(current_ship, c_position.x, c_position.y, current_ship_orientation);
}
