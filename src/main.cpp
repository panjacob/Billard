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

class physical_c {
public:
    std::array<double, 2> position;
    std::array<double, 2> velocity;
    std::array<double, 2> acceleration;

    void update(double dt_f,
                std::function<void(physical_c *, std::array<double, 2> &pos, std::array<double, 2> &vel)> callback_f) {
        using namespace tp::operators;
//        std::cout << "acc: " << acceleration[0] << "," << acceleration[1] << " ||| " << velocity[0] << ","
//                  << velocity[1] << std::endl;
        auto new_position = position + velocity * dt_f + velocity * acceleration * dt_f * dt_f * 0.5;
        auto new_velocity = (velocity + acceleration * dt_f) * 0.94;
        callback_f(this, new_position, new_velocity);
    }
};



int main(int, char **) {
    using namespace std;
    using namespace std::chrono;
    const int width = 640;
    const int height = 480;
    int xMouse, yMouse;
    bool isMouseDown = false;
//    array<int, 2> position = {10, 10};
    milliseconds dt(15);
    steady_clock::time_point current_time = steady_clock::now(); // remember current time
    Cue cue;
    Ball ballWhite(400, 50);

    std::vector<Ball> balls {ballWhite};
    for (int i = 0; i < 6; i++){
        balls.emplace_back(Ball(double(i*50), i*50));
    }


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
        SDL_SetRenderDrawColor(renderer.get(), 255, 128, 128, 255);
        SDL_RenderClear(renderer.get());
        auto keyboardState = SDL_GetKeyboardState(nullptr);
        if (keyboardState[SDL_SCANCODE_LEFT]) cue.intentions["angleL"] = 1;
        if (keyboardState[SDL_SCANCODE_RIGHT]) cue.intentions["angleR"] = 1;
        SDL_GetMouseState(&xMouse, &yMouse);
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

        if (isMouseDown)SDL_SetRenderDrawColor(renderer.get(), 255, 0, 255, 255);
        else SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);


//        SDL_Rect rect = {xMouse - 50, yMouse - 50, 100, 100};
//        SDL_RenderDrawRect(renderer.get(), &rect);

//        rect1_keyboard(keyboardState, &position);
//        SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
//        SDL_Rect rect1 = {position[0], position[1], 100, 100};
//        SDL_RenderDrawRect(renderer.get(), &rect1);

        cue.update_position(xMouse, yMouse);
        cue.update_tracker(balls[0].position[0], balls[0].position[1], renderer, balls[0].radius);
        balls[0].update_cue_collision(renderer, cue);

        for (auto &ball : balls) {
            ball.update_collisions(width, height);

            for (auto &ball2 : balls) {
                if (&ball == &ball2) continue;
                const double distance = sqrt(
                        pow(ball.position[0] - ball2.position[0], 2) + pow(ball.position[1] - ball2.position[1], 2));

                if (distance <= ball.radius * 2) {
                    printf("%f\n", distance);
                    const int angle = atan2(ball.position[0] - ball2.position[0], ball.position[1] - ball2.position[1]);
                    const double ball2_velocity = sqrt(pow(ball2.velocity[0], 2) + pow(ball2.velocity[1], 2));
                    ball.velocity = {sin(angle) * ball2_velocity  , cos(angle)  * ball2_velocity};
                }
            }
            ball.update_position(dt_f);
            ball.render(renderer);
        }

        cue.render(renderer);
        SDL_RenderPresent(renderer.get()); // draw frame to screen
        this_thread::sleep_until(current_time = current_time + dt);
    }

    SDL_Quit();

    return 0;
}






















