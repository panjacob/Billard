#ifndef ___BALL_JK___
#define ___BALL_JK___

class Ball {
public:
    const int radius = 16;
    const int stopSpeed = 25;
    std::array<double, 2> frictionConstant{10, 10};
    const double friction = 0.001;
    std::array<double, 2> position{};
    std::array<double, 2> velocity{0, 0};
    std::array<double, 2> acceleration{0, 0};

    Ball(double x, double y) {
        position = {x, y};
    }

    void update_position(double dt_f) {
        using namespace tp::operators;
        auto new_acceleration = acceleration - (velocity * length(velocity) * friction) ;
        auto new_velocity = velocity + new_acceleration * dt_f;
//      If ball is moving less than minimalSpeed then stop
        if (new_velocity[0] < stopSpeed and new_velocity[1] < stopSpeed and new_velocity[0] > -stopSpeed and
            new_velocity[1] > -stopSpeed)
            new_velocity = {0, 0};
        auto new_position = position + new_velocity * dt_f + new_acceleration * dt_f * dt_f * 0.5;
//        printf("acc: [%.2f : %.2f]   v: [%.2f : %.2f]   pos: [%.2f : %.2f]    dt_f: %.2f\n", acceleration[0],
//               acceleration[1], velocity[0], velocity[1], position[0], position[1], dt_f);
//        acceleration = new_acceleration;
        velocity = new_velocity;
        position = new_position;
    }

    void update_cue_collision(const std::shared_ptr<SDL_Renderer> &renderer, const Cue &cue) {
        if (cue.distanceToWhite <= 0 and velocity[0] == 0 and velocity[1] == 0) {
            acceleration = std::array<double, 2>{cos(cue.angle) * 50000, sin(cue.angle) * 10000};
        } else acceleration = std::array<double, 2>{0, 0};
    }

    void update_collisions(const int &width, const int &height) {
        if (position[0] - radius / 2 <= 0) velocity[0] = -velocity[0];
        else if (position[0] + radius / 2 >= width) velocity[0] = -velocity[0];
        if (position[1] - radius / 2 <= 0) velocity[1] = -velocity[1];
        else if (position[1] + radius / 2 >= height) velocity[1] = -velocity[1];
    }

    void render(const std::shared_ptr<SDL_Renderer> &renderer) {
//        apply_intent();
        SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w; // horizontal offset
                int dy = radius - h; // vertical offset
                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    SDL_RenderDrawPoint(renderer.get(), position[0] + dx, position[1] + dy);
                }
            }
        }
    }
};

#endif