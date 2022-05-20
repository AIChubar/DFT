#include "TextureManager.h"
#include "Game.h"
#include <cstring>

SDL_Texture* TextureManager::loadTexture(const char* texture)
{
    SDL_Surface* tempSurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    
    return tex;
}

void TextureManager::draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
    SDL_RenderCopyEx(Game::renderer, texture, &src, &dest, 0, NULL, flip); // Rendering a texture with an optional animation
}