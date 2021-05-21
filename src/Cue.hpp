#ifndef ___CUE_JK___
#define ___CUE_JK___

class Cue {
public:
    const int height = 16;
    const int width = 100;
    double angle = 0;
    double distanceToWhite = 999;
    std::map<std::string, int> intentions;
    std::array<double, 2> position{};

    Cue() {
        position = {150, 150};
    }

    void apply_intent() {
        if (intentions.count("angleR")) angle += 0.01;
        if (intentions.count("angleL")) angle -= 0.01;

        if (angle > 1) angle = 0;
        else if (angle < 0) angle = 1;

        intentions.clear();
    }

    void update_position(double x, double y) {
        position = {x, y};
    }

    void update_tracker
    (double x, double y, const std::shared_ptr<SDL_Renderer> &renderer, const int &ballRadius) {
        double vx = (x - position[0]);
        double vy = (y - position[1]);
        angle = atan2(vy, vx);
        distanceToWhite = sqrt(pow(vx, 2) + pow(vy, 2)) - width / 2 - ballRadius;
//        printf("%f\n", distanceToWhite);
        SDL_RenderDrawLine(renderer.get(), x, y, position[0], position[1]);

    }

    void render(const std::shared_ptr<SDL_Renderer> &renderer) {
//        apply_intent();
        int x = int(position[0]);
        int y = int(position[1]);
        int h = int(height / 2);
        int w = int(width / 2);
        double anglePI = angle;
//        printf("%f %f\n", angle, anglePI);

        int ULx = x + w * cos(anglePI) - h * sin(anglePI);
        int URx = x - w * cos(anglePI) - h * sin(anglePI);
        int BLx = x + w * cos(anglePI) + h * sin(anglePI);
        int BRx = x - w * cos(anglePI) + h * sin(anglePI);

        int ULy = y + h * cos(anglePI) + w * sin(anglePI);
        int URy = y + h * cos(anglePI) - w * sin(anglePI);
        int BLy = y - h * cos(anglePI) + w * sin(anglePI);
        int BRy = y - h * cos(anglePI) - w * sin(anglePI);

        SDL_SetRenderDrawColor(renderer.get(), 128, 128, 255, 255);
        SDL_RenderDrawLine(renderer.get(), ULx, ULy, URx, URy);
        SDL_RenderDrawLine(renderer.get(), BLx, BLy, BRx, BRy);
        SDL_RenderDrawLine(renderer.get(), ULx, ULy, BLx, BLy);
        SDL_RenderDrawLine(renderer.get(), URx, URy, BRx, BRy);
    }
};

#endif