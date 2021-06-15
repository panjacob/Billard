#ifndef ___BALL_JK___
#define ___BALL_JK___

#include "Cue.hpp"

class Ball {
public:
    double radius;
    int stopSpeed = 1;
    bool collided = false;
    double friction = 0.003;
    std::array<double, 2> position{};
    std::array<double, 2> velocity{};
    std::array<double, 2> acceleration{};
    std::array<int, 3> color{};
    int team{};
    bool isHit = false;

    Ball() = default;

    Ball(double x, double y, double scale, std::array<int, 4> color) {
        position = {x, y};
        radius = 2.85 * scale;
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
        this->team = color[3];
    }

    void update_position(double dt_f) {
        using namespace tp::operators;
        auto new_acceleration = acceleration - velocity * length(velocity) * friction;
        auto new_velocity = velocity + new_acceleration * dt_f;
        auto new_position = position + new_velocity * dt_f + new_acceleration * dt_f * dt_f * 0.5;

        if (length(new_velocity) < stopSpeed) new_velocity = {0, 0};
//        acceleration = new_acceleration;
        velocity = new_velocity;
        position = new_position;
    }

    void update_cue_collision(Cue &cue, bool &tableIsNewTurnCalculated) {
        if (cue.distanceToWhite <= 0 and speed() == 0) {
            tableIsNewTurnCalculated = false;
            isHit = true;
            velocity = {cos(cue.angle) * cue.power, sin(cue.angle) * cue.power};
        }
//        } else acceleration = {0, 0};
    }

    void update_collisions(int x0, int y0, int x1, int y1) {
        if (position[0] - radius <= x0) {
            position[0] = x0 + radius;
            velocity[0] = -velocity[0];
        } else if (position[0] + radius >= x1) {
            position[0] = x1 - radius;
            velocity[0] = -velocity[0];
        }
        if (position[1] - radius <= y0) {
            position[1] = y0 + radius;
            velocity[1] = -velocity[1];
        } else if (position[1] + radius >= y1) {
            position[1] = y1 - radius;
            velocity[1] = -velocity[1];
        }
    }

    double speed() {
        using namespace tp::operators;
        return length(velocity);
    };

    void render(std::shared_ptr<SDL_Renderer> &renderer) {
        SDL_SetRenderDrawColor(renderer.get(), color[0], color[1], color[2], 255);
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w; // horizontal offset
                int dy = radius - h; // vertical offset
                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    SDL_RenderDrawPoint(renderer.get(), position[0] + dx, position[1] + dy);
                }
            }
        }

        if (team == 1) {
            SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
            for (int w = 0; w < radius * 2; w++) {
                for (int h = 0; h < radius * 2; h++) {
                    int dx = radius - w; // horizontal offset
                    int dy = radius - h; // vertical offset
                    if ((dx * dx * 5 + dy * dy) <= (radius * radius)) {
                        SDL_RenderDrawPoint(renderer.get(), position[0] + dx, position[1] + dy);
                    }
                }
            }
        }

    }

};

#endif