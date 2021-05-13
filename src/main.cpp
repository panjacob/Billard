/**
 * This is the simple hello world for SDL2.
 * 
 * You need C++14 to compile this.
 */

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

void rect1_keyboard(const Uint8 *kstate, std::array<int, 2> *position) {
    std::array<int, 2> newPosition = *position;
    printf("oldPos: x: %d   y: %d\n", newPosition[0], newPosition[1]);
    if (kstate[SDL_SCANCODE_LEFT]) {
        newPosition[0] -= 1;
    } else if (kstate[SDL_SCANCODE_RIGHT]) {
        newPosition[0] += 1;
    }
    if (kstate[SDL_SCANCODE_UP]) {
        newPosition[1] -= 1;
    } else if (kstate[SDL_SCANCODE_DOWN]) {
        newPosition[1] += 1;
    }
    printf("newPos: x: %d   y: %d\n", newPosition[0], newPosition[1]);
    *position = newPosition;
}

int main(int, char **) {
    using namespace std;
    using namespace std::chrono;
    int width = 640;
    int height = 480;
    int xMouse, yMouse;
    bool isMouseDown = false;


    errcheck(SDL_Init(SDL_INIT_VIDEO) != 0);

    shared_ptr<SDL_Window> window(
            SDL_CreateWindow("Billard", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
                             SDL_WINDOW_SHOWN), [=](auto w) { SDL_DestroyWindow(w); });

    errcheck(window == nullptr);

    shared_ptr<SDL_Renderer> renderer(SDL_CreateRenderer(
            window.get(), -1, SDL_RENDERER_ACCELERATED), // SDL_RENDERER_PRESENTVSYNC
                                      [=](auto r) { SDL_DestroyRenderer(r); });
    errcheck(renderer == nullptr);

    array<int, 2> position = {10, 10};

    //auto dt = 15ms;
    milliseconds dt(15);

    steady_clock::time_point current_time = steady_clock::now(); // remember current time
    for (bool game_active = true; game_active;) {
        SDL_Event event;
        SDL_SetRenderDrawColor(renderer.get(), 255, 128, 128, 255);
        SDL_RenderClear(renderer.get());
        auto kstate = SDL_GetKeyboardState(NULL);
        SDL_GetMouseState(&xMouse, &yMouse);

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

        if (isMouseDown)SDL_SetRenderDrawColor(renderer.get(), 255, 0, 255, 255);
        else SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);


        SDL_Rect rect = {xMouse - 50, yMouse - 50, 100, 100};
        SDL_RenderDrawRect(renderer.get(), &rect);

        rect1_keyboard(kstate, &position);
        SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
        SDL_Rect rect1 = {position[0], position[1], 100, 100};
        SDL_RenderDrawRect(renderer.get(), &rect1);
//        SDL_Rect destination;
//        destination.x = (100 /2) - (destination.w /2) ;
//        destination.y = (100 /2) - (destination.h /2) ;
//        SDL_RenderCopyEx(renderer.get(), rect1, NULL, &destination, 50.f , NULL, SDL_FLIP_NONE);


        SDL_RenderPresent(renderer.get()); // draw frame to screen
        this_thread::sleep_until(current_time = current_time + dt);
    }
    SDL_Quit();
    return 0;
}

/*

		auto currentTime = std::chrono::high_resolution_clock::now();
//		std::this_thread::sleep_for( dt );
//		std::this_thread::sleep_until( prevTime + dt );
//		prevTime = prevTime + dt;// lub std::chrono::high_resolution_clock::now();
		dt = currentTime - prevTime;
		prevTime = currentTime;
		static int f = 0;
		if (((f++)%100) == 0)
		std::cout << "dt = " << dt.count() << "  FPS=" << (1/dt.count()) << std::endl;

*/






















