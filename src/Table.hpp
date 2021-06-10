#ifndef ___TABLE_JK___
#define ___TABLE_JK___

#include <algorithm>
#include <random>
#include "Ball.hpp"
#include "Pocket.hpp"


class Table {

public:
    int scale, x0, x1, y0, y1, margin, tableHeight, tableWidth;
    double ballRadius;
    std::vector<Ball> balls;
    std::vector<Pocket> pockets;
    std::array<int, 2> points{};
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
        placePockets();
    }

    std::vector<std::array<int, 4>> generateBalls() {
        std::vector<std::array<int, 4>> ball_colors{
                {255, 255, 0,   0},//yellow
                {255, 255, 0,   1},
                {0,   0,   255, 0},//blue
                {0,   0,   255, 1},
                {255, 0,   0,   0},//red
                {255, 0,   0,   1},
                {102, 0,   102, 0},//purple
                {102, 0,   102, 1},
                {255, 102, 0,   0}, //orange
                {255, 102, 0,   1},
                {0,   204, 0,   0}, //green
                {0,   204, 0,   1},
                {102, 51,  0,   0}, //brown
                {102, 51,  0,   1},
                {0,   0,   0,   2}, //black
        };

        std::shuffle(ball_colors.begin(), ball_colors.end(), std::mt19937(std::random_device()()));
        return ball_colors;
    }

    void placeBalls() {
        const double ballDiameter = ballRadius * 2;
        const double kitchen_y = (y1 - y0) / 2 + margin / 2;
        const double kitchen_x = x1 - ((y1 - y0) / 2);
        const double first_row = x0 + tableHeight / 4;
        std::vector<std::array<int, 4>> ball_colors = generateBalls();

        std::array<int, 4> ball_white{255, 255, 255, 3};

        balls.emplace_back(Ball(kitchen_x, kitchen_y, scale, ball_white));

        for (int i = -2; i <= 2; i++) {
            const double distance = ballDiameter * i;
            balls.emplace_back(Ball(first_row, kitchen_y - distance, scale, ball_colors[i + 2]));
        }

        const double second_row = first_row + 2.85 * scale * 2;
        for (int i = 1; i <= 4; i++) {
            const double distance = (ballDiameter * i) - 2 * ballDiameter - ballRadius;
            balls.emplace_back(Ball(second_row, kitchen_y - distance, scale, ball_colors[i + 4]));
        }

        const double third_row = second_row + 2.85 * scale * 2;
        for (int i = -1; i <= 1; i++) {
            const double distance = ballRadius * 2 * i;
            balls.emplace_back(Ball(third_row, kitchen_y - distance, scale, ball_colors[i + 10]));
        }

        const double fourth_row = third_row + 2.85 * scale * 2;
        for (int i = 1; i <= 2; i++) {
            const double distance = (ballDiameter * i) - ballDiameter - ballRadius;
            balls.emplace_back(Ball(fourth_row, kitchen_y - distance, scale, ball_colors[i + 11]));
        }
        const double fifth_row = fourth_row + 2.85 * scale * 2;
        balls.emplace_back(Ball(fifth_row, kitchen_y, scale, ball_colors[14]));
    }

    void placePockets() {
        pockets.emplace_back(Pocket(x0, y0, scale));
        pockets.emplace_back(Pocket(x0, y1, scale));
        pockets.emplace_back(Pocket(x1, y0, scale));
        pockets.emplace_back(Pocket(x1, y1, scale));
        pockets.emplace_back(Pocket((x0 + x1) / 2, y0, scale));
        pockets.emplace_back(Pocket((x0 + x1) / 2, y1, scale));
    }

    void renderTable(const std::shared_ptr<SDL_Renderer> &renderer) const {
        SDL_SetRenderDrawColor(renderer.get(), 255, 255, 0, 255);
        SDL_RenderDrawLine(renderer.get(), x0, y0, x1, y0);
        SDL_RenderDrawLine(renderer.get(), x0, y0, x0, y1);
        SDL_RenderDrawLine(renderer.get(), x0, y1, x1, y1);
        SDL_RenderDrawLine(renderer.get(), x1, y0, x1, y1);

        SDL_SetRenderDrawColor(renderer.get(), 44, 130, 87, 255);
        for (int i = x0 + 1; i < x1; i++)
            SDL_RenderDrawLine(renderer.get(), i, y0 + 1, i, y1);
    }

    void renderBalls(const std::shared_ptr<SDL_Renderer> &renderer) {
        for (auto &ball : balls) ball.render(renderer);
    }

    void renderPockets(const std::shared_ptr<SDL_Renderer> &renderer) {
        for (auto &pocket : pockets) pocket.render(renderer);
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

                if (distance <= ball.radius * 2) {
                    ball.collided = true;
                    ball2.collided = true;
                    const double angle = atan2(ball.position[0] - ball2.position[0],
                                               ball.position[1] - ball2.position[1]);
                    double ball_new_velocity = ball2.speed() * collision_ratio + ball.speed() * (1 - collision_ratio);
                    double ball2_new_velocity = ball.speed() * collision_ratio + ball2.speed() * (1 - collision_ratio);
                    ball.velocity = {sin(angle) * ball_new_velocity, cos(angle) * ball_new_velocity};
                    ball2.velocity = {-sin(angle) * ball2_new_velocity, -cos(angle) * ball2_new_velocity};
                }
            }
        }
        for (auto &ball : balls) ball.update_position(dt_f);
    }

    void updatePocketCollisions() {
        for (auto &pocket : pockets) {
            for (unsigned int i = 0; i <= balls.size(); i++) {
                auto &ball = balls[i];
                const double distance = sqrt(
                        pow(ball.position[0] - pocket.position[0], 2) + pow(ball.position[1] - pocket.position[1], 2));
                if (distance < pocket.radius + ballRadius / 2) {
                    if (ball.team != 3)
                        balls.erase(balls.begin() + i);
                    if (ball.team == 0) points[0] += 1;
                    else if (ball.team == 1) points[1] += 1;
//                  else if(ball.team == 2)  if black then oppsite player wins
//                  else place white ball where player wants to
                    printf("%d - %d\n", points[0], points[1]);
                }
            }
        }
    }


    void render(const std::shared_ptr<SDL_Renderer> &renderer) {
        renderTable(renderer);
        renderPockets(renderer);
        renderBalls(renderer);

    }


};

#endif