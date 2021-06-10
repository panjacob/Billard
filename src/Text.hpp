//#ifndef ___TEXT_JK___
//#define ___TEXT_JK___
//
//#include <stdlib.h>
//
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>
//
//class Text {
//public:
//    SDL_Texture *texture1, *texture2;
//
//
//
//    Text() {
//
//    }
//
//    void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
//                           TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
//        int text_width;
//        int text_height;
//        SDL_Surface *surface;
//        SDL_Color textColor = {255, 255, 255, 0};
//
//        surface = TTF_RenderText_Solid(font, text, textColor);
//        *texture = SDL_CreateTextureFromSurface(renderer, surface);
//        text_width = surface->w;
//        text_height = surface->h;
//        SDL_FreeSurface(surface);
//        rect->x = x;
//        rect->y = y;
//        rect->w = text_width;
//        rect->h = text_height;
//    }
//
//
//    void render(const std::shared_ptr <SDL_Renderer> &renderer) {
//
//    }
//};
//
//#endif