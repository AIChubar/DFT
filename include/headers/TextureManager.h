#pragma once
#include "sdl2/SDL.h"
#include "sdl2/SDL_image.h"


class TextureManager {

public:
    static SDL_Texture* loadTexture(const char* filename);
    static void draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
};
