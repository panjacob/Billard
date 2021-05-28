#ifndef ___TABLE_JK___
#define ___TABLE_JK___

#include "Ball.hpp"

class Table {

public:
    int scale, x0, x1, y0, y1, margin, tableHeight, tableWidth;
    double ballRadius;
    std::vector<Ball> balls{};
    const double collision_ratio = 0.8;


    Table(int width, int height, int margin) {
        this->margin = margin;
        tableHeight = height - margin;
        tableWidth = width - margin;
        scale = tableHeight / 127;
        x0 = margin / 2;
        y0 = margin / 2;
        x1 = tableHeight * 2 + margin / 2;
        y1 = tableHeight + margin / 2;
        ballRadius = 2.85 * scale;

        placeBalls();
    }

    void placeBalls() {
        const double ballDiameter = ballRadius * 2;
        const double kitchen_y = (y1 - y0) / 2 + margin / 2;
        const double kitchen_x = x1 - ((y1 - y0) / 2);
        const double first_row = x0 + tableHeight / 4;

        balls.emplace_back(Ball(kitchen_x, kitchen_y, scale));
        for (int i = -2; i <= 2; i++) {
            const double distance = ballDiameter * i;
            balls.emplace_back(Ball(first_row, kitchen_y - distance, scale));
        }

        const double second_row = first_row + 2.85 * scale * 2;
        for (int i = 1; i <= 4; i++) {
            const double distance = (ballDiameter * i) - 2 * ballDiameter - ballRadius;
            balls.emplace_back(Ball(second_row, kitchen_y - distance, scale));
        }

        const double third_row = second_row + 2.85 * scale * 2;
        for (int i = -1; i <= 1; i++) {
            const double distance = ballRadius * 2 * i;
            balls.emplace_back(Ball(third_row, kitchen_y - distance, scale));
        }

        const double fourth_row = third_row + 2.85 * scale * 2;
        for (int i = 1; i <= 2; i++) {
            const double distance = (ballDiameter * i) - ballDiameter - ballRadius;
            balls.emplace_back(Ball(fourth_row, kitchen_y - distance, scale));
        }
        const double fifth_row = fourth_row + 2.85 * scale * 2;
        balls.emplace_back(Ball(fifth_row, kitchen_y, scale));
    }

    void renderTable(const std::shared_ptr<SDL_Renderer> &renderer) {
        SDL_SetRenderDrawColor(renderer.get(), 255, 255, 0, 255);
        SDL_RenderDrawLine(renderer.get(), x0, y0, x1, y0);
        SDL_RenderDrawLine(renderer.get(), x0, y0, x0, y1);
        SDL_RenderDrawLine(renderer.get(), x0, y1, x1, y1);
        SDL_RenderDrawLine(renderer.get(), x1, y0, x1, y1);
    }

    void renderBalls(const std::shared_ptr<SDL_Renderer> &renderer) {
        for (auto &ball : balls) ball.render(renderer);
    }

    void updateBallCollisions(double dt_f) {
        using namespace tp::operators;
        for (auto &ball : balls) ball.collided = false;

        for (auto &ball : balls) {
            ball.update_collisions(x0, y0, x1, y1);
            if (ball.collided) continue;
            if (ball.speed() == 0) continue;
            for (auto &ball2 : balls) {
                if (ball2.collided) continue;
                if (&ball == &ball2) continue;

                const double distance = sqrt(
                        pow(ball.position[0] - ball2.position[0], 2) + pow(ball.position[1] - ball2.position[1], 2));

//                printf("%f\n", ball.speed());

                if (distance <= ball.radius * 2) {
                    ball.collided = true;
                    ball2.collided = true;
                    const double angle = atan2(ball.position[0] - ball2.position[0], ball.position[1] - ball2.position[1]);
                    double ball_new_velocity = ball2.speed() * collision_ratio + ball.speed() * (1 - collision_ratio);
                    double ball2_new_velocity =ball.speed() * collision_ratio + ball2.speed() * (1 - collision_ratio);
                    ball.velocity = {sin(angle) * ball_new_velocity, cos(angle) * ball_new_velocity};
                    ball2.velocity = {-sin(angle) * ball2_new_velocity, -cos(angle) * ball2_new_velocity};
                    printf("%f\n", angle);
                }
            }
        }
//        printf("NEW \n");
        for (auto &ball : balls) ball.update_position(dt_f);
    }


    void render(const std::shared_ptr<SDL_Renderer> &renderer) {
        renderTable(renderer);
        renderBalls(renderer);


    }


};

#endif