#pragma once
#include <SDL2\SDL.h>

class Drawable{
    public:
        virtual ~Drawable(){}
        virtual void draw(SDL_Renderer* renderer) = 0;
};