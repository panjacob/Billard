#ifndef ___CUE_JK___
#define ___CUE_JK___

class Cue {
public:
    int height = 16;
    int width = 100;
    int basePower = 500000;
    int distance;
    int baseDistance = 100;
    double distanceToWhite = 999;
    double power;
    double powerRatio = 0;
    double powerRatioIncrease = 1;
    double angle = 0;

    bool isHitProcedure = false;
    bool isMouseDown = false;
    double scale;

    std::map<std::string, int> intentions;
    std::array<double, 2> position{};

    explicit Cue(double scale) {
        position = {150, 150};
        this->scale = scale;
        this->power = basePower;
        this->distance = baseDistance;
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

    void update_position(bool isMouseDown, double dt_f) {
        this->isMouseDown = isMouseDown;

        if (isMouseDown && powerRatio < 1) powerRatio += powerRatioIncrease * dt_f;
        else if (!isMouseDown && powerRatio > 0) isHitProcedure = true;
        else if (!isHitProcedure) powerRatio = 0;

        power = basePower * powerRatio;
        distance = baseDistance * (powerRatio + 1.0);


        printf("powerRatio: %f    dtf: %f\n", powerRatio, distanceToWhite);
    }

    void update_tracker
            (double ballX, double ballY, double mouseX, double mouseY, double &ballRadius,
             const std::shared_ptr<SDL_Renderer> &renderer) {
//        position = {mouseX, mouseY};
        double vx = (ballX - mouseX);
        double vy = (ballY - mouseY);
        angle = atan2(vy, vx);


        position = {ballX - cos(angle) * distance, ballY - sin(angle) * distance};

        distanceToWhite = sqrt(pow(ballX - position[0], 2) + pow(ballY - position[1], 2)) - width / 2 - ballRadius;
        SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer.get(), ballX, ballY, ballX + vx, ballY + vy);

    }

    void render(const std::shared_ptr<SDL_Renderer> &renderer) {
        int x = int(position[0]);
        int y = int(position[1]);
        int h = int(height / 2);
        int w = int(width / 2);

        SDL_SetRenderDrawColor(renderer.get(), 128, 90, 70, 255);
//        else SDL_SetRenderDrawColor(renderer.get(), 128, 110, 70, 255);
        for (; w > 0; w--) {
            for (; h > 0; h--) {
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
    }
};

#endif