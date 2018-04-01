#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cassert>

#include "sdl_app.h"

const int width  = 640;
const int height = 480;

SDL_Context::SDL_Context(char* fn)
    : m_labels_filename(fn)
{
    update_labels();
    init();
}

SDL_Context::SDL_Context(const label_container& labels)
    : m_text_labels(labels)
{
    init();
}

SDL_Context::~SDL_Context()
{
    SDL_DestroyWindow(m_screen);
    TTF_CloseFont(m_font);
    TTF_Quit();
    SDL_Quit();
}

int event_filter(void* unused, SDL_Event* ev)
{
    return ev->type == SDL_QUIT;
}

void SDL_Context::update_labels()
{
    m_text_labels = get_labels();
}

label_container SDL_Context::get_labels()
{
    std::ifstream ifs(m_labels_filename);
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
    label_container labels;
    std::copy_n(res.begin(), num_labels, labels.begin());
    return labels;
}

void SDL_Context::init()
{
    SDL_Init(SDL_INIT_VIDEO);
    if(TTF_Init() == -1) {
        throw std::runtime_error("Couldn't init SDL_ttf\n");
    }
    m_font = TTF_OpenFont("DejaVuSans.ttf", 36);
    if(!m_font) {
        throw std::runtime_error("Couldn't load font\n");
    }
    m_screen = SDL_CreateWindow("Bus schedule",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640, 480,
            0);
    if(!m_screen) {
        throw std::runtime_error("Could not create window\n");
    }
    m_renderer = SDL_CreateRenderer(m_screen, -1, SDL_RENDERER_SOFTWARE);
    if(!m_renderer) {
        throw std::runtime_error("Could not create renderer\n");
    }

    m_col_white.r = 255;
    m_col_white.g = 255;
    m_col_white.b = 255;
    m_col_white.a = 255;

    m_col_yellow.r = 255;
    m_col_yellow.g = 255;
    m_col_yellow.b = 0;
    m_col_yellow.a = 255;

    m_rects[0].x = 10;
    m_rects[0].y = 10;
    m_rects[1].x = width * 0.8;
    m_rects[1].y = 10;
    for(int i = 0; i < (num_labels - 2) / 3; i++) {
        m_rects[i * 3 + 2].x = 10;
        m_rects[i * 3 + 2].y = 80 + 55 * i;
        m_rects[i * 3 + 3].x = 100;
        m_rects[i * 3 + 3].y = 80 + 55 * i;
        m_rects[i * 3 + 4].x = width * 0.8;
        m_rects[i * 3 + 4].y = 80 + 55 * i;
    }

    SDL_SetEventFilter(event_filter, NULL);
}

void SDL_Context::run()
{
    while(1) {
        bool cont = run(m_text_labels, 10000);
        if(!cont)
            return;
        try {
            update_labels();
        } catch (std::runtime_error& e) {
            printf("Could not update labels\n");
        }
    }
}

bool SDL_Context::run(const label_container& labels, int ms)
{
    m_text_labels = labels;
    SDL_RenderClear(m_renderer);
    for(int i = 0; i < num_labels; i++) {
        auto col = i < 2 ? m_col_yellow : m_col_white;
        SDL_Surface *text = TTF_RenderUTF8_Blended(m_font, m_text_labels[i].c_str(), col);
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, text);
        SDL_FreeSurface(text);
        text = NULL;
        SDL_QueryTexture(texture, NULL, NULL, &m_rects[i].w, &m_rects[i].h);
        if(i % 3 == 1) {
            m_rects[i].x = width - m_rects[i].w - 10;
        }
        SDL_RenderCopy(m_renderer, texture, NULL, &m_rects[i]);
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
    SDL_RenderPresent(m_renderer);
    SDL_Event e;
    int have_ev = SDL_WaitEventTimeout(&e, ms);
    if(have_ev) {
        assert(e.type == SDL_QUIT);
        return false;
    }
    return true;
}

