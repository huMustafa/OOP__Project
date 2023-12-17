#pragma once
#include "game.hpp"
#include <stdio.h>
#include <iostream>

// Constructor for the Game class
Game::Game(void) {
    // Initialize SDL, SDL_image, and SDL_ttf
    int imgFlags = IMG_INIT_PNG;
    if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) || (!(IMG_Init(imgFlags) & imgFlags)) || (TTF_Init() == -1)) {
        std::cout << "Could not initialize SDL! Error: " << SDL_GetError() << std::endl;
    }
    else {
        // Create a window and renderer
        g_window = SDL_CreateWindow("Battleship", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (g_window == NULL) {
            std::cout << "Couldn't create the window! SDL Error:" << SDL_GetError() << std::endl;
        }
        g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (g_renderer == NULL) {
            std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        }

        // Set renderer color and viewport
        SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        g_viewport = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderSetViewport(g_renderer, &g_viewport);

        // Initialize SDL_mixer and set up players' enemies
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }
        player_1.set_enemy(&player_2);
        player_2.set_enemy(&player_1);

        // Load textures, fonts, and sounds
        ships_spritesheet = load_texture("assets\\Sprites\\BattleShipSheet_final.png");
        tokens_spritesheet = load_texture("assets\\Sprites\\Tokens.png");
        radar_image = load_texture("assets\\Grids\\radargrid_final.png");
        ocean_image = load_texture("assets\\Grids\\oceangrid_final.png");
        metalic_panel = load_texture("assets\\Grids\\metalic_panel.png");
        g_font = load_font("assets\\fonts\\militar_font.ttf");
        // Load prompt textures
        prompt_textures[PLAYER_1_SET_SHIP_POSITIONS] = load_text_texture("PLAYER ONE: Position your Ships!");
        prompt_textures[PLAYER_2_SET_SHIP_POSITIONS] = load_text_texture("PLAYER TWO: Position your Ships!");
        prompt_textures[PLAYER_1_TURN_TO_ATTACK] = load_text_texture("PLAYER ONE: Turn to Attack!");
        prompt_textures[PLAYER_2_TURN_TO_ATTACK] = load_text_texture("PLAYER TWO: Turn to Attack!");
        prompt_textures[PLAYER_1_WINS] = load_text_texture("PLAYER ONE: YOU'VE WON!");
        prompt_textures[PLAYER_2_WINS] = load_text_texture("PLAYER TWO: YOU'VE WON!");
        // Load sounds and music
        water_splash = load_sound("assets\\Sfx\\water_splash.wav");
        explosion = load_sound("assets\\Sfx\\explosion_1.wav");
        good_beep = load_sound("assets\\Sfx\\good_beep.wav");
        denide_action = load_sound("assets\\Sfx\\bad_beep.wav");
        background_music = load_music("assets\\Sfx\\background_music.wav");

        // Set textures for players and initialize their ships
        player_1.set_textures(ocean_image, radar_image, tokens_spritesheet);
        player_2.set_textures(ocean_image, radar_image, tokens_spritesheet);

        // Set positions for the banner and metalic panel
        g_banner = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - GRID_TEXTURE_HEIGHT };
        metalic_panel_position = { RADAR_X_POS, RADAR_Y_POS, GRID_TEXTURE_WIDTH, GRID_TEXTURE_HEIGHT };
    }
}

// Destructor for the Game class
Game::~Game(void) {
    // Free resources and quit SDL components
    SDL_DestroyTexture(ships_spritesheet);
    SDL_DestroyTexture(tokens_spritesheet);
    SDL_DestroyTexture(radar_image);
    SDL_DestroyTexture(ocean_image);
    for (size_t i = 0; i < TOTAL_TEXT_TEXTURES; i++) {
        SDL_DestroyTexture(prompt_textures[i]);
        prompt_textures[i] = NULL;
    }
    ships_spritesheet = NULL;
    tokens_spritesheet = NULL;
    radar_image = NULL;
    ocean_image = NULL;
    TTF_CloseFont(g_font);
    g_font = NULL;
    Mix_FreeChunk(water_splash);
    Mix_FreeChunk(explosion);
    Mix_FreeChunk(good_beep);
    Mix_FreeChunk(denide_action);
    water_splash = NULL;
    explosion = NULL;
    good_beep = NULL;
    denide_action = NULL;
    Mix_FreeMusic(background_music);
    background_music = NULL;
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    g_window = NULL;
    g_renderer = NULL;
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

// Load a text texture with a specific font
SDL_Texture* Game::load_text_texture(const char* text) {
    SDL_Surface* text_surface = TTF_RenderText_Solid(g_font, text, BLACK_COLOR);
    SDL_Texture* t_texture = nullptr;
    if (text_surface == NULL) {
        std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
    else {
        t_texture = SDL_CreateTextureFromSurface(g_renderer, text_surface);
        if (t_texture == NULL) {
            std::cout << "Unable to create texture from rendered text! SDL Error:" << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(text_surface);
    }
    return t_texture;
}

// Load an image texture
SDL_Texture* Game::load_texture(const char* path_to_file) {
    SDL_Texture* new_texture = IMG_LoadTexture(g_renderer, path_to_file);
    if (new_texture == NULL) {
        std::cout << "Could not load the image, Error: " << path_to_file << IMG_GetError() << std::endl;
    }
    return new_texture;
}

// Load a font
TTF_Font* Game::load_font(const char* path_to_file) {
    TTF_Font* new_font = TTF_OpenFont(path_to_file, 28);
    if (new_font == NULL) {
        std::cout << "Could not load the font! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
    return new_font;
}

// Load a sound
Mix_Chunk* Game::load_sound(const char* path_to_file) {
    Mix_Chunk* new_sound = Mix_LoadWAV(path_to_file);
    if (new_sound == NULL) {
        std::cout << "Couldn't load the sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
    return new_sound;
}

// Load music
Mix_Music* Game::load_music(const char* path_to_file) {
    Mix_Music* new_music = Mix_LoadMUS(path_to_file);
    if (new_music == NULL) {
        std::cout << "Couldn't load the sound! SDL_mixer Error:" << Mix_GetError() << std::endl;
    }
    return new_music;
}

// Main game loop
void Game::game_loop(void) {
    // Play background music and initialize cursors
    Mix_PlayMusic(background_music, 0);
    Cursor p1_ship_cursor(&player_1, OCEAN_MODE, 1, 5);
    Cursor p2_ship_cursor(&player_2, OCEAN_MODE, 1, 5);
    Cursor p1_play_cursor(&player_2, RADAR_MODE, 1, 1);
    Cursor p2_play_cursor(&player_1, RADAR_MODE, 1, 1);
    p1_ship_cursor.set_texture(tokens_spritesheet);
    p1_play_cursor.set_texture(tokens_spritesheet);
    p2_ship_cursor.set_texture(tokens_spritesheet);
    p2_play_cursor.set_texture(tokens_spritesheet);

    // Initialize players' ships and set up the game
    player_1.init_ships(ships_spritesheet);
    player_2.init_ships(ships_spritesheet);
    game_finished = false;

    // Player 1 sets ship positions
    while (player_is_selecting_ship_positions(&p1_ship_cursor) && !game_finished) {
        SDL_RenderClear(g_renderer);
        draw_banner(PLAYER_1_SET_SHIP_POSITIONS);
        player_1.draw_ocean(g_renderer);
        draw_metal_panel();
        player_1.draw_ships(g_renderer, TOTAL_SHIPS);
        SDL_RenderPresent(g_renderer);
    }

    // Player 2 sets ship positions
    while (player_is_selecting_ship_positions(&p2_ship_cursor) && !game_finished) {
        SDL_RenderClear(g_renderer);
        draw_banner(PLAYER_2_SET_SHIP_POSITIONS);
        player_2.draw_ocean(g_renderer);
        draw_metal_panel();
        player_2.draw_ships(g_renderer, TOTAL_SHIPS);
        SDL_RenderPresent(g_renderer);
    }        

    this_turn = PLAYER_1_TURN;
    Cursor *this_turn_cursor = &p1_play_cursor;
    while (player_is_selecting_target_to_bomb(this_turn_cursor) && !game_finished){
        SDL_RenderClear(g_renderer);
        switch (this_turn){
        case PLAYER_1_TURN:
            this_turn_cursor = &p1_play_cursor;
            draw_banner(PLAYER_1_TURN_TO_ATTACK);
            player_1.draw_ocean(g_renderer);
            player_1.draw_ships(g_renderer, TOTAL_SHIPS);
            player_1.draw_ocean_tiles(g_renderer);
            player_2.draw_radar(g_renderer);
            player_2.draw_radar_tiles(g_renderer);
            break;
        
        default:
            this_turn_cursor = &p2_play_cursor;
            draw_banner(PLAYER_2_TURN_TO_ATTACK);
            player_2.draw_ocean(g_renderer);
            player_2.draw_ships(g_renderer, TOTAL_SHIPS);
            player_2.draw_ocean_tiles(g_renderer);
            player_1.draw_radar(g_renderer);
            player_1.draw_radar_tiles(g_renderer);
            break;
        }

        this_turn_cursor->draw(g_renderer);
        if (player_1.won())
            victory_screen(PLAYER_1);
        else if(player_2.won())
            victory_screen(PLAYER_2);
        SDL_RenderPresent(g_renderer);
    }
}

void Game::victory_screen(players winner){
    text_prompts message;
    if (winner == PLAYER_1)
        message = PLAYER_1_WINS;
    else
        message = PLAYER_2_WINS;
    SDL_RenderClear(g_renderer);
    draw_banner(message);
    SDL_RenderPresent(g_renderer);

    Uint32 start_time = SDL_GetTicks();
    game_finished = false;
    while(!game_finished){
        while (SDL_PollEvent(&g_event) != 0 ){
            switch (get_event_code())
            {
                case QUIT_GAME:
                    game_finished = true;
                    break;
            }
        }
        if((SDL_GetTicks() - start_time) >= 5000){ //5 seconds delay before exiting the game
            game_finished = true;
        }
        SDL_Delay(10);
     }
    
}

game_event Game::get_event_code(void){
    switch (g_event.type){
        case SDL_QUIT:
            return QUIT_GAME;

        case SDL_KEYDOWN:
            switch( g_event.key.keysym.sym ){
                case SDLK_w:
                case SDLK_UP:
                    return PRESS_UP;

                case SDLK_s:
                case SDLK_DOWN:
                    return PRESS_DOWN;

                case SDLK_a:
                case SDLK_LEFT:
                    return PRESS_LEFT;

                case SDLK_d:
                case SDLK_RIGHT:
                    return PRESS_RIGHT;

                case SDLK_RETURN:
                    return PRESS_ENTER;

                case SDLK_SPACE:
                    return PRESS_ACTION;
            }
        
        default:
            return NOTHING;
    }
}

bool Game::player_is_selecting_target_to_bomb(Cursor *current_cursor){
    while (SDL_PollEvent(&g_event) != 0 ){
        switch (get_event_code())
        {
            case QUIT_GAME:
                game_finished = true;
                break;

            case PRESS_UP:
                if(current_cursor->move_up()){
                    Mix_PlayChannel(-1, good_beep, 0);
                }   
                else
                    Mix_PlayChannel(-1, denide_action, 0);
                break;

            case PRESS_DOWN:
                if(current_cursor->move_down()){
                    Mix_PlayChannel(-1, good_beep, 0);
                }
                else
                    Mix_PlayChannel(-1, denide_action, 0);
                break;

            case PRESS_RIGHT:
                if(current_cursor->move_right()){
                    Mix_PlayChannel(-1, good_beep, 0);
                }
                else
                    Mix_PlayChannel(-1, denide_action, 0);
                break;

            case PRESS_LEFT:
                if(current_cursor->move_left()){
                    Mix_PlayChannel(-1, good_beep, 0);
                }
                else
                    Mix_PlayChannel(-1, denide_action, 0);
                break;

            case PRESS_ACTION:
            case PRESS_ENTER:
                Mix_PlayChannel(-1, good_beep, 0);

                switch (current_cursor->try_bomb_enemy()){
                    case ALREADY_BOMBED:
                        Mix_PlayChannel(-1, denide_action, 0);
                        break;

                    case EXPLODED:
                        Mix_PlayChannel(-1, explosion, 0);

                        break;
                    
                    default:
                        Mix_PlayChannel(-1, water_splash, 0);
                        change_turn();
                        break;
                }

                break;
        }
    }
    return true;
}

bool Game::player_is_selecting_ship_positions(Cursor *current_cursor){
    while (SDL_PollEvent(&g_event) != 0 ){
        switch (get_event_code()){
            case QUIT_GAME:
                game_finished = true;
                break;

            case PRESS_UP:
                if(current_cursor->move_up()){
                    Mix_PlayChannel(-1, good_beep, 0);
                    current_cursor->set_ship_position();
                }   
                else
                    Mix_PlayChannel(-1, denide_action, 0);
                break;

            case PRESS_DOWN:
                if(current_cursor->move_down()){
                    Mix_PlayChannel(-1, good_beep, 0);
                    current_cursor->set_ship_position();
                }
                else
                    Mix_PlayChannel(-1, denide_action, 0);
                break;

            case PRESS_RIGHT:
                if(current_cursor->move_right()){
                    Mix_PlayChannel(-1, good_beep, 0);
                    current_cursor->set_ship_position();
                }
                else
                    Mix_PlayChannel(-1, denide_action, 0);
                break;

            case PRESS_LEFT:
                if(current_cursor->move_left()){
                    Mix_PlayChannel(-1, good_beep, 0);
                    current_cursor->set_ship_position();
                }
                else
                    Mix_PlayChannel(-1, denide_action, 0);
                break;

            case PRESS_ACTION:
                if(current_cursor->toggle_ship_orientation()){
                    Mix_PlayChannel(-1, good_beep, 0);
                    current_cursor->set_ship_position();
                }
                else
                    Mix_PlayChannel(-1, denide_action, 0);
                
                break;

            case PRESS_ENTER:
                Mix_PlayChannel(-1, good_beep, 0);

                current_cursor->assign_ship_position();
                current_cursor->next_ship();

                if(current_cursor->current_ship_in_use() == TOTAL_SHIPS)
                    return false;
                break;
        }
    }
    return true;
}

void Game::change_turn(void){
    if (this_turn == PLAYER_1_TURN)
        this_turn = PLAYER_2_TURN;
    else
        this_turn = PLAYER_1_TURN;
}

void Game::init(void){
    game_loop();
}

void Game::draw_banner(text_prompts prompt){
    SDL_RenderCopy(g_renderer, metalic_panel, NULL, &g_banner);
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(g_renderer, &g_banner);
    prompt_position = {300, 10, 750, 60};
    SDL_RenderCopy(g_renderer, prompt_textures[prompt], NULL, &prompt_position);
}

void Game:: draw_metal_panel(void){
    SDL_RenderCopy(g_renderer, metalic_panel, NULL, &metalic_panel_position);
}