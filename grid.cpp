#pragma once

#include <SDL2\SDL.h>
#include "grid.hpp"

// Constructor for the Grid class
Grid::Grid(void) {
    // Initialize player_grid with default values
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            player_grid[y][x] = { x, y, false, false };
        }
    }

    // Set initial positions and score
    radar_position = { RADAR_X_POS, RADAR_Y_POS, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT };
    ocean_position = { OCEAN_X_POS, OCEAN_Y_POS, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT };
    score = 0;
}

// Destructor for the Grid class
Grid::~Grid(void) {
    radar_texture = NULL;
    ocean_texture = NULL;
}

// Set the enemy grid for interaction
void Grid::set_enemy(Grid* e) {
    enemy = e;
}

// Set textures for radar, ocean, and tiles
void Grid::set_textures(SDL_Texture* ocean, SDL_Texture* radar, SDL_Texture* tiles) {
    ocean_texture = ocean;
    radar_texture = radar;
    tokens_spritesheet = tiles;
}

// Draw radar, ocean, and ships on the renderer
void Grid::draw_radar(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, radar_texture, NULL, &radar_position);
}

void Grid::draw_ocean(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, ocean_texture, NULL, &ocean_position);
}

void Grid::draw_ships(SDL_Renderer* renderer, ship_type ship_to_draw) {
    // Draw specific ship or all ships based on ship_to_draw parameter
    if (ship_to_draw == TOTAL_SHIPS) {
        player_ships.draw(renderer, CARRIER);
        player_ships.draw(renderer, BATTLESHIP);
        player_ships.draw(renderer, CRUISER);
        player_ships.draw(renderer, SUBMARINE);
        player_ships.draw(renderer, DESTROYER);
    }
    else {
        player_ships.draw(renderer, ship_to_draw);
    }
}

// Draw radar tiles based on the state of each tile
void Grid::draw_radar_tiles(SDL_Renderer* renderer) {
    SDL_Rect tile_position = { 0,0,0,0 };
    for (int y_coord = 0; y_coord < GRID_HEIGHT; y_coord++) {
        for (int x_coord = 0; x_coord < GRID_WIDTH; x_coord++) {
            if (player_grid[y_coord][x_coord].bombed && player_grid[y_coord][x_coord].has_ship) {
                tile_position = { TILES_RADAR_INIT_X_POS + (x_coord * TILE_SIZE), TILES_RADAR_INIT_Y_POS + (y_coord * TILE_SIZE), TILE_SIZE, TILE_SIZE };
                SDL_RenderCopy(renderer, tokens_spritesheet, &TILE_SPRITES.red, &tile_position);
            }
            else if (player_grid[y_coord][x_coord].bombed) {
                tile_position = { TILES_RADAR_INIT_X_POS + (x_coord * TILE_SIZE), TILES_RADAR_INIT_Y_POS + (y_coord * TILE_SIZE), TILE_SIZE, TILE_SIZE };
                SDL_RenderCopy(renderer, tokens_spritesheet, &TILE_SPRITES.green, &tile_position);
            }
        }
    }
}

// Draw ocean tiles based on the state of each tile
void Grid::draw_ocean_tiles(SDL_Renderer* renderer) {
    SDL_Rect tile_position = { 0,0,0,0 };
    for (int y_coord = 0; y_coord < GRID_HEIGHT; y_coord++) {
        for (int x_coord = 0; x_coord < GRID_WIDTH; x_coord++) {
            if (player_grid[y_coord][x_coord].bombed && player_grid[y_coord][x_coord].has_ship) {
                tile_position = { TILES_OCEAN_INIT_X_POS + (x_coord * TILE_SIZE), TILES_OCEAN_INIT_Y_POS + (y_coord * TILE_SIZE), TILE_SIZE, TILE_SIZE };
                SDL_RenderCopy(renderer, tokens_spritesheet, &TILE_SPRITES.bombed, &tile_position);
            }
            else if (player_grid[y_coord][x_coord].bombed) {
                tile_position = { TILES_OCEAN_INIT_X_POS + (x_coord * TILE_SIZE), TILES_OCEAN_INIT_Y_POS + (y_coord * TILE_SIZE), TILE_SIZE, TILE_SIZE };
                SDL_RenderCopy(renderer, tokens_spritesheet, &TILE_SPRITES.water, &tile_position);
            }
        }
    }
}

// Bomb a specific tile and update the score
tile_state Grid::bomb_tile(int x, int y) {
    if (player_grid[y - 1][x - 1].bombed) {
        return ALREADY_BOMBED;
    }
    else if (player_grid[y - 1][x - 1].has_ship) {
        player_grid[y - 1][x - 1].bombed = true;
        enemy->score += 1;
        return EXPLODED;
    }
    else {
        player_grid[y - 1][x - 1].bombed = true;
        return WATER_SPLASHED;
    }
}

// Bomb a tile in the enemy grid
bool Grid::bomb_enemy_tile(int x, int y) {
    return enemy->bomb_tile(x, y);
}

// Initialize the ships in the grid with the provided ships spritesheet
void Grid::init_ships(SDL_Texture* ships_spritesheet) {
    player_ships.set_spritesheet(ships_spritesheet);
}

// Set the position of a ship on the grid
void Grid::set_ship_position(ship_type ship, int x_pos, int y_pos, ship_orientation orientation) {
    x_pos -= 1;
    y_pos -= 1;

    // Set ship position and orientation
    player_ships.set_position(ship, x_pos, y_pos);
    player_ships.set_orientation(ship, orientation);
    player_ships.set_current_position(ship);
}

// Assign ship to tiles in the grid
void Grid::assign_ship_to_tiles(ship_type ship, int x_pos, int y_pos, ship_orientation orientation) {
    x_pos -= 1;
    y_pos -= 1;
    int ship_length;
    switch (ship) {
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
    // Mark tiles with a ship based on the ship's orientation
    if (orientation == UP || orientation == DOWN) {
        for (size_t i = 0; i < ship_length; i++) {
            player_grid[y_pos + i][x_pos].has_ship = true;
        }
    }
    else {
        for (size_t i = 0; i < ship_length; i++) {
            player_grid[y_pos][x_pos + i].has_ship = true;
        }
    }
}

// Check if a tile has a ship
bool Grid::tile_has_ship(int x, int y) {
    return player_grid[y - 1][x - 1].has_ship;
}

// Check if a tile is bombed
bool Grid::tile_is_bombed(int x, int y) {
    return player_grid[y - 1][x - 1].bombed;
}

// Check if the player has won the game
bool Grid::won(void) {
    if (score > 16)
        return true;
    return false;
}
