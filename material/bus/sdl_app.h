#pragma once

#include <array>
#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

constexpr int num_labels = 23;

typedef std::array<std::string, num_labels> label_container;

class SDL_Context {
    public:
        SDL_Context(const label_container& labels);
        SDL_Context(char* fn);
        ~SDL_Context();
        void run();
        bool run(const label_container& labels, int ms);

    private:
        void update_labels();
        label_container get_labels();
        void init();

        TTF_Font* m_font;
        SDL_Window* m_screen;
        SDL_Renderer* m_renderer;
        SDL_Color m_col_white;
        SDL_Color m_col_yellow;
        std::array<SDL_Rect, num_labels> m_rects;
        label_container m_text_labels;
        std::string m_labels_filename;
};


