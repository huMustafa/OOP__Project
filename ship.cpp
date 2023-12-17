#pragma once
#include "ship.hpp"

// Constructor for the Ship_sheet class
Ship_sheet::Ship_sheet(void) {
    // Initialize ship_list with default values for each ship type
    ship_list[CARRIER] = { 0, 0, CARRIER, UP, CARRIER_SPRITES, &CARRIER_SPRITES.up, 0 };
    set_current_position(CARRIER);

    ship_list[BATTLESHIP] = { 10, 10, BATTLESHIP, UP, BATTLESHIP_SPRITES, &BATTLESHIP_SPRITES.up, 0 };
    set_current_position(BATTLESHIP);

    ship_list[CRUISER] = { 10, 10, CRUISER, UP, CRUISER_SPRITES, &CRUISER_SPRITES.up, 0 };
    set_current_position(CRUISER);

    ship_list[SUBMARINE] = { 10, 10, SUBMARINE, UP, SUBMARINE_SPRITES, &SUBMARINE_SPRITES.up, 0 };
    set_current_position(SUBMARINE);

    ship_list[DESTROYER] = { 10, 10, DESTROYER, UP, DESTROYER_SPRITES, &DESTROYER_SPRITES.up, 0 };
    set_current_position(DESTROYER);
}

// Destructor for the Ship_sheet class
Ship_sheet::~Ship_sheet(void) {
    s_texture = NULL;
}

// Convert index to ship type
ship_type Ship_sheet::index_to_type(int i) {
    switch (i) {
    case 0:
        return CARRIER;
    case 1:
        return BATTLESHIP;
    case 2:
        return CRUISER;
    case 3:
        return SUBMARINE;
    default:
        return DESTROYER;
    }
}

// Set the spritesheet for the ships
void Ship_sheet::set_spritesheet(SDL_Texture* texture) {
    s_texture = texture;
}

// Set the position of a specific ship
void Ship_sheet::set_position(ship_type s, int x, int y) {
    ship ship_to_set = ship_list[s];
    ship_to_set.x_pos = x;
    ship_to_set.y_pos = y;
    ship_list[s] = ship_to_set;
}

// Set the current position of a specific ship on the screen
void Ship_sheet::set_current_position(ship_type s) {
    ship ship_to_set = ship_list[s];
    ship_to_set.screen_position.x = TILES_OCEAN_INIT_X_POS + (ship_to_set.x_pos * SHIP_TILE_SIZE);
    ship_to_set.screen_position.y = TILES_OCEAN_INIT_Y_POS + (ship_to_set.y_pos * SHIP_TILE_SIZE);
    int ship_length;
    switch (ship_to_set.type) {
    case CARRIER:
        ship_length = 5;
        break;
    case BATTLESHIP:
        ship_length = 4;
        break;
    case CRUISER:
    case SUBMARINE:
    default:
        ship_length = 3;
        break;
    case DESTROYER:
        ship_length = 2;
        break;
    }

    // Set the width and height of the ship's screen position based on its orientation
    if (ship_to_set.orientation == UP || ship_to_set.orientation == DOWN) {
        ship_to_set.screen_position.h = ship_length * SHIP_TILE_SIZE;
        ship_to_set.screen_position.w = SHIP_TILE_SIZE;
    }
    else {
        ship_to_set.screen_position.w = ship_length * SHIP_TILE_SIZE;
        ship_to_set.screen_position.h = SHIP_TILE_SIZE;
    }
    ship_list[s] = ship_to_set;
}

// Set the orientation of a specific ship
void Ship_sheet::set_orientation(ship_type s, ship_orientation orientation) {
    ship* ship_to_set = &ship_list[s];
    ship_to_set->orientation = orientation;
    switch (orientation) {
    case UP:
        ship_to_set->current_sprite = &(ship_to_set->sprites.up);
        break;
    case DOWN:
        ship_to_set->current_sprite = &(ship_to_set->sprites.down);
        break;
    case RIGHT:
        ship_to_set->current_sprite = &(ship_to_set->sprites.right);
        break;
    default:
        ship_to_set->current_sprite = &(ship_to_set->sprites.left);
        break;
    }
}

// Draw a specific ship on the renderer
void Ship_sheet::draw(SDL_Renderer* renderer, ship_type s) {
    ship ship_to_render = ship_list[s];
    SDL_RenderCopy(renderer, s_texture, ship_to_render.current_sprite, &(ship_to_render.screen_position));
}
