#ifndef ___TABLE_JK___
#define ___TABLE_JK___
#include "Ball.hpp"
class Table {
public:
    int scale, x0, x1, y0, y1;
    std::vector <Ball> balls{};




    Table(int width, int height, int margin) {
        const int tableHeight = height - margin;
        scale = tableHeight / 127;


        x0 = margin / 2;
        y0 = margin / 2;
        x1 = tableHeight * 2 + margin / 2;
        y1 = tableHeight + margin / 2;

        const double kitchen_y = (y1-y0)/2 + margin/2;
        const double kitchen_x = x1-((y1-y0)/2);

        balls.emplace_back(Ball(kitchen_x, kitchen_y, scale));
        for (int i = 0; i <= 15; i++) {
            balls.emplace_back(Ball(double(i * 50), i * 50, scale));
        }
    }

    void renderTable(const std::shared_ptr <SDL_Renderer> &renderer) {
        SDL_SetRenderDrawColor(renderer.get(), 255, 255, 0, 255);
        SDL_RenderDrawLine(renderer.get(), x0, y0, x1, y0);
        SDL_RenderDrawLine(renderer.get(), x0, y0, x0, y1);
        SDL_RenderDrawLine(renderer.get(), x0, y1, x1, y1);
        SDL_RenderDrawLine(renderer.get(), x1, y0, x1, y1);
    }

    void renderBalls(const std::shared_ptr <SDL_Renderer> &renderer) {
        for (auto &ball : balls) ball.render(renderer);
    }

    void updateBallCollisions(double dt_f){
        for (auto &ball : balls) ball.collided = false;

        for (auto &ball : balls) {
            ball.update_collisions(x0, y0, x1, y1);
            if (ball.collided) continue;
            for (auto &ball2 : balls) {
                if (&ball == &ball2 || ball2.collided) continue;
                const double distance = sqrt(
                        pow(ball.position[0] - ball2.position[0], 2) + pow(ball.position[1] - ball2.position[1], 2));

                if (distance <= ball.radius * 2) {
                    ball.collided = true;
                    ball2.collided = true;
//                    printf("%f\n", distance);
                    const int angle = atan2(ball.position[0] - ball2.position[0], ball.position[1] - ball2.position[1]);
                    double ball_velocity = sqrt(pow(ball.velocity[0], 2) + pow(ball.velocity[1], 2));
                    double ball2_velocity = sqrt(pow(ball2.velocity[0], 2) + pow(ball2.velocity[1], 2));

                    ball.velocity = {sin(angle) * ball2_velocity, cos(angle) * ball2_velocity};
                    ball2.velocity = {-sin(angle) * ball_velocity, -cos(angle) * ball_velocity};
                }
            }
            ball.update_position(dt_f);
//            ball.render(renderer);
        }
    }


    void render(const std::shared_ptr <SDL_Renderer> &renderer) {
        renderTable(renderer);
        renderBalls(renderer);




    }


};

#endif