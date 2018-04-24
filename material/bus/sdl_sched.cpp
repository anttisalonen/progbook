#include <stdio.h>
#include <string>
#include <stdexcept>

#include "SDL.h"
#include "SDL_ttf.h"

class SDL_Schedule {
	public:
		SDL_Schedule(const std::string& filename);
		~SDL_Schedule();
		void render();

	private:
		TTF_Font *font;
		SDL_Window *screen;
		SDL_Renderer *renderer;
		SDL_Color col_white;
		SDL_Rect dest;
};

SDL_Schedule::SDL_Schedule(const std::string& filename)
{
    SDL_Init(SDL_INIT_VIDEO);
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Could not init SDL: %s\n", SDL_GetError());
		throw std::runtime_error("error");
    }
    if(TTF_Init() != 0) {
        fprintf(stderr, "Couldn't init SDL_ttf: %s\n", TTF_GetError());
		throw std::runtime_error("error");
    }
    font = TTF_OpenFont("DejaVuSans.ttf", 72);
    if(!font) {
        fprintf(stderr, "Couldn't load font\n");
		throw std::runtime_error("error");
    }
    screen = SDL_CreateWindow("My application",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          640, 480,
                          0);
    if(!screen) {
        fprintf(stderr, "Could not create window\n");
		throw std::runtime_error("error");
    }
    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_SOFTWARE);
    if(!renderer) {
        fprintf(stderr, "Could not create renderer\n");
		throw std::runtime_error("error");
    }

    col_white.r = 255;
    col_white.g = 255;
    col_white.b = 255;
    col_white.a = 255;
}

SDL_Schedule::~SDL_Schedule()
{
    SDL_DestroyWindow(screen);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

void SDL_Schedule::render()
{
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
}

int main(int argc, char** argv)
{
	if(argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return 1;
	}
	SDL_Schedule sched(argv[1]);
	sched.render();
    SDL_Delay(3000);

    return 0;
}
