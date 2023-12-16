#pragma once
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>

enum players{
    PLAYER_1,
    PLAYER_2,
    TOTAL_PLAYERS
};

enum game_event{
    QUIT_GAME,
    PRESS_UP,
    PRESS_DOWN,
    PRESS_RIGHT,
    PRESS_LEFT,
    PRESS_ACTION,
    PRESS_ENTER,
    NOTHING
};

enum ship_type{
    CARRIER,// 5 squares long
    BATTLESHIP,// 4 squares long          
    CRUISER,// 3 squares long
    SUBMARINE,// 3 squares long
    DESTROYER,// 2 squares long
    TOTAL_SHIPS
};

enum ship_orientation{
    UP,
    DOWN,
    RIGHT,
    LEFT,
    TOTAL_POSITIONS
};

enum grid_mode{
    RADAR_MODE,
    OCEAN_MODE
};

enum text_prompts{
    PLAYER_1_SET_SHIP_POSITIONS,
    PLAYER_2_SET_SHIP_POSITIONS,
    PLAYER_1_TURN_TO_ATTACK,
    PLAYER_2_TURN_TO_ATTACK,
    PLAYER_1_WINS,
    PLAYER_2_WINS,
    TOTAL_TEXT_TEXTURES
};

enum Turn{
    PLAYER_1_TURN,
    PLAYER_2_TURN
};

enum tile_state{
    ALREADY_BOMBED,
    EXPLODED,
    WATER_SPLASHED
};

const int TOTAL_ORIENTATIONS = 4;
struct ship_sprite_source{
    SDL_Rect down;
    SDL_Rect up;
    SDL_Rect left;
    SDL_Rect right;
};

const int TOTAL_TOKENS = 4;
struct token_sprite_source{
    SDL_Rect green;
    SDL_Rect red;
    SDL_Rect water;
    SDL_Rect bombed;
};

struct ship{
    int x_pos;
    int y_pos;
    ship_type type;
    ship_orientation orientation;
    ship_sprite_source sprites;
    SDL_Rect *current_sprite;
    SDL_Rect screen_position;
};

struct tile{
    int x;
    int y;
    bool bombed;
    bool has_ship;
};

struct position{
    int x;
    int y;

    bool operator==(const position& other) const {
        return (x == other.x) && (y == other.y);
    }

    bool operator!=(const position& other) const {
        return !(*this == other);
    }
};

Turn this_turn;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 10;

const int SHIP_TILE_SIZE = 58;
const int TILE_SIZE = 58;

const int GRID_TEXTURE_WIDTH = 640;
const int GRID_TEXTURE_HEIGHT = 640;

const int OCEAN_X_POS = 0;
const int OCEAN_Y_POS = SCREEN_HEIGHT - GRID_TEXTURE_HEIGHT;

const int RADAR_X_POS = GRID_TEXTURE_WIDTH;
const int RADAR_Y_POS = SCREEN_HEIGHT - GRID_TEXTURE_HEIGHT;

const int TILES_OCEAN_INIT_X_POS = OCEAN_X_POS + SHIP_TILE_SIZE + 1;
const int TILES_OCEAN_INIT_Y_POS = OCEAN_Y_POS + SHIP_TILE_SIZE + 1;

const int TILES_RADAR_INIT_X_POS = RADAR_X_POS + SHIP_TILE_SIZE + 1;
const int TILES_RADAR_INIT_Y_POS = RADAR_Y_POS + SHIP_TILE_SIZE + 1;

const int CARRIER_WIDTH = 1;
const int CARRIER_HEIGHT = 5;

const int BATTLESHIP_WIDTH = 1;
const int BATTLESHIP_HEIGHT = 4;

const int CRUISER_WIDTH = 1;
const int CRUISER_HEIGHT = 3;

const int SUBMARINE_WIDTH = 1;
const int SUBMARINE_HEIGHT = 3;

const int DESTROYER_WIDTH = 1;
const int DESTROYER_HEIGHT = 2;

SDL_Color BLACK_COLOR = {0, 0, 0};

ship_sprite_source CARRIER_SPRITES = {
    {150,       0,      32,    156},     // UP
    {150,       244,    32,    156},     // DOWN
    {244,       0,      156,    32},     // LEFT
    {244,       368,    156,    32}      // RIGHT
};

ship_sprite_source BATTLESHIP_SPRITES = {
    {112,       0,      32,    125},     // UP
    {112,       275,    32,    125},     // DOWN
    {275,       38,     125,    32},   // LEFT
    {244,       330,    125,    32}     //RIGHT
};

ship_sprite_source CRUISER_SPRITES = {
    {75,       0,      32,    94},     // UP
    {75,       306,    32,    94},     // DOWN
    {306,      75,     94,    32},   // LEFT
    {244,      293,    94,    32}     //RIGHT
};

ship_sprite_source SUBMARINE_SPRITES = {
    {38,       0,      32,    94},     // UP
    {38,       306,    32,    94},     // DOWN
    {306,      112,      94,    32},   // LEFT
    {244,      256,    94,    32}     //RIGHT
};

ship_sprite_source DESTROYER_SPRITES = {
    {0,         0,      32,    63},     // UP
    {0,         337,    32,    63},     // DOWN
    {335,       150,    63,    32},   // LEFT
    {244,       218,    63,    32}     //RIGHT
};

token_sprite_source TILE_SPRITES ={
    {0,     0,      32,     32},
    {32,    0,      32,     32},
    {64,    0,      32,     32},
    {96,    0,      32,     32}
};