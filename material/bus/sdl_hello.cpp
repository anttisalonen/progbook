#include <stdio.h>

#include "SDL.h"
#include "SDL_ttf.h"

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	if(TTF_Init() == -1) {
		fprintf(stderr, "Couldn't init SDL_ttf\n");
		return 1;
	}
	TTF_Font *font = TTF_OpenFont("DejaVuSans.ttf", 72);
	if(!font) {
		fprintf(stderr, "Couldn't load font\n");
		return 1;
	}
	SDL_Window *screen = SDL_CreateWindow("My Game Window",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          640, 480,
                          0);
	if(!screen) {
		fprintf(stderr, "Could not create window\n");
		return 1;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_SOFTWARE);
	if(!renderer) {
		fprintf(stderr, "Could not create renderer\n");
		return 1;
	}

	SDL_Color col_white;
	col_white.r = 255;
	col_white.g = 255;
	col_white.b = 255;
	col_white.a = 255;
	SDL_Surface *text = TTF_RenderUTF8_Blended(font, "Hello world", col_white);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
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
	SDL_RenderPresent(renderer);
	SDL_Delay(3000);

	SDL_DestroyWindow(screen);
	TTF_CloseFont(font);
	font = NULL;
	TTF_Quit();
	SDL_Quit();
	return 0;
}
