#ifndef ___BALL_TP_PJATK_HPP___
#define ___BALL_TP_PJATK_HPP___

class Ball {
public:
    const int radius = 16;
//    double angle = 0;
    std::map<std::string, int> intentions{};
    std::array<double, 2> position{};

    Ball() {
        position = {150, 150};
    }

//    void apply_intent() {
//        if (intentions.count("angleR")) angle += 0.01;
//        if (intentions.count("angleL")) angle -= 0.01;
//
//        if (angle > 1) angle = 0;
//        else if (angle < 0) angle = 1;
//
//        intentions.clear();
//    }

    void update_position(double x, double y) {
        position = {x, y};
    }

//    void update_tracker(double x, double y, const std::shared_ptr <SDL_Renderer> &renderer) {
//        double vx = (x - position[0]);
//        double vy = (y - position[1]);
//        angle = atan2(vy, vx);
////        printf("%f %f | %f\n", vx, vy, angle);
//        SDL_RenderDrawLine(renderer.get(), x, y, position[0], position[1]);
//    }

    void render(const std::shared_ptr <SDL_Renderer> &renderer) {
//        apply_intent();
        SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
        for (int w = 0; w < radius * 2; w++)
        {
            for (int h = 0; h < radius * 2; h++)
            {
                int dx = radius - w; // horizontal offset
                int dy = radius - h; // vertical offset
                if ((dx*dx + dy*dy) <= (radius * radius))
                {
                    SDL_RenderDrawPoint(renderer.get(), position[0] + dx, position[1] + dy);
                }
            }
        }
    }
};

#endif