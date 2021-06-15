#ifndef ___CUE_JK___
#define ___CUE_JK___

class Cue {
public:
    int height = 16;
    int width = 100;
    int basePower = 500;
    int distance;
    int baseDistance = 100;
    double distanceToWhite = 999;
    double power;
    double powerRatio = 0;
    double powerRatioIncrease = 2;
    double angle = 0;
    std::array<int, 3> color{};

    bool isHitProcedure = false;
//    bool isMouseDown = false;
    double scale;

    std::map<std::string, int> intentions;
    std::array<double, 2> position{};

    explicit Cue(double scale, std::array<int, 3> color) {
        position = {150, 150};
        this->scale = scale;
        this->power = basePower;
        this->distance = baseDistance;
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
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


    void hitProcedure(double dt_f) {
        if (distanceToWhite >= 0) {
            distance -= baseDistance * (powerRatio + 1) * dt_f * 3;
        } else {
            isHitProcedure = false;
            distance = baseDistance;
            powerRatio = 0;
        }
    }

    void update
            (double &ballX, double &ballY, double &ballRadius, double mouseX, double mouseY, bool isMouseDown,
             double dt_f,
             std::shared_ptr<SDL_Renderer> &renderer) {
        angle = angle_f(ballX, ballY, mouseX, mouseY, renderer);
        position = position_f(ballX, ballY);
        distanceToWhite = distanceToWhite_f(ballX, ballY, ballRadius);

        if (isHitProcedure) {
            hitProcedure(dt_f);
            return;
        }
        if (!isMouseDown && powerRatio > 0) isHitProcedure = true;
        else if (isMouseDown && powerRatio <= 1) powerRatio += powerRatioIncrease * dt_f / 3;
        else powerRatio = 0;

        power = basePower * powerRatio;
        distance = baseDistance * (powerRatio + 1.0);
    }

    double distanceToWhite_f(double ballX, double ballY, double ballRadius) const {
        return sqrt(pow(ballX - position[0], 2) + pow(ballY - position[1], 2)) - width / 2 - ballRadius;
    }

    std::array<double, 2> position_f(double ballX, double ballY) const {
        return {ballX - cos(angle) * distance, ballY - sin(angle) * distance};
    }

    double angle_f(double ballX, double ballY, double mouseX, double mouseY, std::shared_ptr<SDL_Renderer> &renderer) {
        double vx = (ballX - mouseX);
        double vy = (ballY - mouseY);
        SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer.get(), ballX, ballY, ballX + vx, ballY + vy);

        return angle = atan2(vy, vx);
    }

    void render(const std::shared_ptr<SDL_Renderer> &renderer) {
        int x = int(position[0]);
        int y = int(position[1]);
        int h = int(height / 2);
        int w = int(width / 2);

        SDL_SetRenderDrawColor(renderer.get(), color[0], color[1], color[2], 255);
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