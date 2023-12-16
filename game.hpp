#pragma once

#include <SDL2\SDL_image.h>
#include <SDL2\SDL.h>
#include <SDL2\SDL_mixer.h>
#include <SDL2\SDL_ttf.h>
#include "cursor.hpp"
#include "constants_and_data_types.hpp"
#include "grid.hpp"
#include "ship.hpp"


class Game{
    public:
        Game(void);
        ~Game(void);
        void init(void);
        game_event get_event_code(void);
        bool player_is_selecting_ship_positions(Cursor *current_cursor);
        bool player_is_selecting_target_to_bomb(Cursor *current_cursor);
        void game_loop(void);
        void clean_up(void);
        void draw_banner(text_prompts prompt);
        void draw_metal_panel(void);
        void change_turn(void);
        void victory_screen(players winner);
        SDL_Texture *load_texture(const char *path_to_file);
        SDL_Texture *load_text_texture(const char *text);
        TTF_Font *load_font(const char *path_to_file);
        Mix_Chunk *load_sound(const char *path_to_file);
        Mix_Music *load_music(const char *path_to_file);
    private:
        bool game_finished;
        Grid player_1;
        Grid player_2;
        SDL_Window *g_window;
        SDL_Renderer *g_renderer;
        SDL_Rect g_viewport;
        SDL_Event g_event;
        TTF_Font *g_font;
        SDL_Texture *prompt_textures[TOTAL_TEXT_TEXTURES];
        SDL_Rect prompt_position;
        Mix_Chunk *water_splash;
        Mix_Chunk *explosion;
        Mix_Chunk *good_beep;
        Mix_Chunk *denide_action;
        Mix_Music *background_music;
        SDL_Rect g_banner;
        SDL_Rect metalic_panel_position;
        SDL_Texture *ships_spritesheet;
        SDL_Texture *radar_image;
        SDL_Texture *ocean_image;
        SDL_Texture *tokens_spritesheet;
        SDL_Texture *metalic_panel;
};