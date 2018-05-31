#include <stdio.h>
#include <string>
#include <stdexcept>
#include <array>
#include <fstream>
#include <sstream>
#include <vector>

#include "SDL.h"
#include "SDL_ttf.h"

const int width  = 640;
const int height = 480;
const int num_labels = 23;

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
        SDL_Color col_yellow;
        SDL_Rect dest;
        std::string file;
        std::array<SDL_Rect, num_labels> rects;
};

SDL_Schedule::SDL_Schedule(const std::string& filename)
    : file(filename)
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
    font = TTF_OpenFont("DejaVuSans.ttf", 36);
    if(!font) {
        fprintf(stderr, "Couldn't load font\n");
        throw std::runtime_error("error");
    }
    screen = SDL_CreateWindow("My application",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width, height,
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

    col_yellow.r = 255;
    col_yellow.g = 255;
    col_yellow.b = 0;
    col_yellow.a = 255;

    rects[0].x = 10;
    rects[0].y = 10;
    rects[1].y = 10;
    for(int i = 0; i < (num_labels - 2) / 3; i++) {
        rects[i * 3 + 2].x = 10;
        rects[i * 3 + 2].y = 80 + 55 * i;
        rects[i * 3 + 3].x = 100;
        rects[i * 3 + 3].y = 80 + 55 * i;
        rects[i * 3 + 4].y = 80 + 55 * i;
    }

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
    std::ifstream ifs(file);
    std::string contents((std::istreambuf_iterator<char>(ifs)),
            (std::istreambuf_iterator<char>()));
    std::stringstream ss(contents);
    std::string to;
    std::vector<std::string> res;
    while(std::getline(ss, to, '\n')) {
        res.push_back(to);
    }
    if(res.size() != num_labels) {
        throw std::runtime_error("Invalid number of labels\n");
    }

    SDL_RenderClear(renderer);
    for(int i = 0; i < num_labels; i++) {
        auto col = i < 2 ? col_yellow : col_white;
        SDL_Surface *text = TTF_RenderUTF8_Blended(font, res[i].c_str(), col);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_FreeSurface(text);
        text = NULL;
        SDL_QueryTexture(texture, NULL, NULL, &rects[i].w, &rects[i].h);
        if(i % 3 == 1) {
            rects[i].x = width - rects[i].w - 10;
        }
        SDL_RenderCopy(renderer, texture, NULL, &rects[i]);
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
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
