#include <stdio.h>

#include "SDL.h"
#include "SDL_ttf.h"

int main()
{
    /* TODO: init SDL */
    if(TTF_Init() != 0) {
        fprintf(stderr, "Couldn't init SDL_ttf: %s\n", TTF_GetError());
        return 1;
    }
    TTF_Font *font = TTF_OpenFont("DejaVuSans.ttf", 72);
    if(!font) {
        fprintf(stderr, "Couldn't load font\n");
        return 1;
    }
    /* TODO: create SDL_Window and SDL_Renderer */

    SDL_Color col_white;
    col_white.r = 255;
    col_white.g = 255;
    col_white.b = 255;
    col_white.a = 255;
    SDL_Surface *text = TTF_RenderUTF8_Blended(font, "Hello world", col_white);

    /* TODO: clear the renderer */
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text);
    SDL_FreeSurface(text);
    text = NULL;
    SDL_Rect dest;
    dest.x = 10;
    dest.y = 10;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    texture = NULL;
    /* TODO: present, delay and clean up both SDL and TTF libraries */

    return 0;
