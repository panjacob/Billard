#ifndef ___CUE_JK___
#define ___CUE_JK___

class Cue {
public:
    const int height = 16;
    const int width = 100;
    const int base_power = 100000;
    double power;
    double angle = 0;
    double distanceToWhite = 999;
    double scale;
    std::map<std::string, int> intentions;
    std::array<double, 2> position{};

    explicit Cue(double scale) {
        position = {150, 150};
        this->scale = scale;
        this->power = base_power;
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

    void update_tracker
            (double x, double y, const std::shared_ptr<SDL_Renderer> &renderer, const int &ballRadius) {
        double vx = (x - position[0]);
        double vy = (y - position[1]);
        angle = atan2(vy, vx);
        distanceToWhite = sqrt(pow(vx, 2) + pow(vy, 2)) - width / 2 - ballRadius;
        SDL_RenderDrawLine(renderer.get(), x, y, x + vx, y + vy);

    }

    void render(const std::shared_ptr<SDL_Renderer> &renderer) {
        int x = int(position[0]);
        int y = int(position[1]);
        int h = int(height / 2);
        int w = int(width / 2);

        SDL_SetRenderDrawColor(renderer.get(), 128, 90, 70, 255);
        for(; h > 0; h--){
            int ULx = x + w * cos(angle) - h * sin(angle);
            int URx = x - w * cos(angle) - h * sin(angle);
            int BLx = x + w * cos(angle) + h * sin(angle);
            int BRx = x - w * cos(angle) + h * sin(angle);

            int ULy = y + h * cos(angle) + w * sin(angle);
            int URy = y + h * cos(angle) - w * sin(angle);
            int BLy = y - h * cos(angle) + w * sin(angle);
            int BRy = y - h * cos(angle) - w * sin(angle);

            SDL_RenderDrawLine(renderer.get(), ULx, ULy, URx, URy);
            SDL_RenderDrawLine(renderer.get(), BLx, BLy, BRx, BRy);
            SDL_RenderDrawLine(renderer.get(), ULx, ULy, BLx, BLy);
            SDL_RenderDrawLine(renderer.get(), URx, URy, BRx, BRy);
        }


    }
};

#endif