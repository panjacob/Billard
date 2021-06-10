//Import biblioteki do wypisywania tekstu
#include <SDL2/SDL.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <vector>
#include <array>
#include <map>
#include <functional>
#include "vectors.hpp"
#include "Cue.hpp"
#include "Ball.hpp"
#include "Table.hpp"
//#include "Text.hpp"
#include "Pocket.hpp"
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
// check for errors
#define errcheck(e)                   \
  {                                   \
    if (e)                            \
    {                                 \
      cout << SDL_GetError() << endl; \
      SDL_Quit();                     \
      return -1;                      \
    }                                 \
  }

//void rect1_keyboard(const Uint8 *kstate, std::array<int, 2> *position) {
//    std::array<int, 2> newPosition = *position;
////    printf("oldPos: x: %d   y: %d\n", newPosition[0], newPosition[1]);
//    if (kstate[SDL_SCANCODE_LEFT]) {
//        newPosition[0] -= 1;
//    } else if (kstate[SDL_SCANCODE_RIGHT]) {
//        newPosition[0] += 1;
//    }
//    if (kstate[SDL_SCANCODE_UP]) {
//        newPosition[1] -= 1;
//    } else if (kstate[SDL_SCANCODE_DOWN]) {
//        newPosition[1] += 1;
//    }
////    printf("newPos: x: %d   y: %d\n", newPosition[0], newPosition[1]);
//    *position = newPosition;
//}

//#include <unistd.h>
//std::string getcwd_string(  ) {
//    char buff[128];
//    getcwd( buff, 128 );
//    std::string cwd( buff );
//    return cwd;
//}

//void draw(const char* msg, int x, int y, int r, int g, int b, int size, const std::shared_ptr<SDL_Renderer> &ren) {
//
//    SDL_Surface* surf;
//    SDL_Texture* tex;
//    SDL_Color color;
//    color.r=r;
//    color.g=g;
//    color.b=b;
//    color.a=255;
//    SDL_Rect rect;
//    TTF_Font *font = TTF_OpenFont("data/font.ttf", size);
//    surf = TTF_RenderText_Solid(font, msg, color);
//    tex = SDL_CreateTextureFromSurface(ren.get(), surf);
//    rect.x=x;
//    rect.y=y;
//    rect.w=surf->w;
//    rect.h=surf->h;
//    SDL_FreeSurface(surf);
//    SDL_RenderCopy(ren.get(), tex, nullptr, &rect);
//    SDL_DestroyTexture(tex);
//}


int main(int, char **) {
    using namespace std;
    using namespace std::chrono;
    const int width = 2560;
    const int height = 1080;
    const int margin = 300;
//    std::string pathxd = getcwd_string();
//    printf("DUUUUUUUUUUUPPPPPPPPPPPPPPPPPPPPPAAAAAAAAAAA\n");
//    printf("%s\n", pathxd.c_str());


    int mouseX, mouseY;
    bool isMouseDown = false;
    milliseconds dt(15);
    steady_clock::time_point current_time = steady_clock::now(); // remember current time
    Table table(width, height, margin);
    Cue cue(table.scale);


    errcheck(SDL_Init(SDL_INIT_VIDEO) != 0);
    shared_ptr<SDL_Window> window(
            SDL_CreateWindow("Billard", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
                             SDL_WINDOW_SHOWN), [=](auto w) { SDL_DestroyWindow(w); });
    errcheck(window == nullptr);
    shared_ptr<SDL_Renderer> renderer(SDL_CreateRenderer(
            window.get(), -1, SDL_RENDERER_ACCELERATED), // SDL_RENDERER_PRESENTVSYNC
                                      [=](auto r) { SDL_DestroyRenderer(r); });
    errcheck(renderer == nullptr);


    for (bool game_active = true; game_active;) {
        SDL_Event event;
        SDL_SetRenderDrawColor(renderer.get(), 0, 40, 73, 255);
        SDL_RenderClear(renderer.get());
//        auto keyboardState = SDL_GetKeyboardState(nullptr);
//        if (keyboardState[SDL_SCANCODE_LEFT]) cue.intentions["angleL"] = 1;
//        if (keyboardState[SDL_SCANCODE_RIGHT]) cue.intentions["angleR"] = 1;
        SDL_GetMouseState(&mouseX, &mouseY);
        double dt_f = dt.count() / 1000.0;

        while (SDL_PollEvent(&event)) { // check if there are some events
            switch (event.type) {
                case SDL_QUIT:
                    game_active = false;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) game_active = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    isMouseDown = true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    isMouseDown = false;
                    break;
            }
        }

//        if (isMouseDown)SDL_SetRenderDrawColor(renderer.get(), 255, 0, 255, 255);
//        else SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);

        cue.update_position(isMouseDown, dt_f);

        table.balls[0].update_cue_collision(cue);
        table.updateBallCollisions(dt_f);
        table.updatePocketCollisions();
        table.render(renderer);
//        cue.update_tracker(table.balls[0].position[0], table.balls[0].position[1], renderer, table.balls[0].radius);
        cue.update_tracker(table.balls[0].position[0], table.balls[0].position[1], mouseX, mouseY,
                           table.balls[0].radius, renderer);
        cue.render(renderer);

        SDL_RenderPresent(renderer.get()); // draw frame to screen
        this_thread::sleep_until(current_time = current_time + dt);
    }

    SDL_Quit();

    return 0;
}






















