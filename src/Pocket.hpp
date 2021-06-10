#ifndef ___POCKET_JK___
#define ___POCKET_JK___

class Pocket {
public:
    double radius = 2.85;
    std::array<double, 2> position{};

    Pocket(double x, double y, double scale) {
        position = {x, y};
        radius = 6.5 * scale;
    }

    void update_position() {
//        using namespace tp::operators;
//        auto new_acceleration = acceleration - velocity * length(velocity) * friction;
//        auto new_velocity = velocity + new_acceleration * dt_f;
//        auto new_position = position + new_velocity * dt_f + new_acceleration * dt_f * dt_f * 0.5;
//
//        if (length(new_velocity) < stopSpeed) new_velocity = {0, 0};
//        velocity = new_velocity;
//        position = new_position;
    }


    void render(const std::shared_ptr<SDL_Renderer> &renderer) {
        SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
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