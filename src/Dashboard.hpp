#ifndef ___DASHBOARD_JK___
#define ___DASHBOARD_JK___

#include "Image.hpp"

class Dashboard {
public:
    std::vector<Image> imagesPlayers;
    std::vector<Image> numbers;



    Dashboard(const std::shared_ptr<SDL_Renderer> &renderer) {
        Image playerOne("data/playerone.bmp", 1800, 300, 300, 100, renderer);
        Image playerTwo("data/playertwo.bmp", 1800, 400, 300, 100, renderer);
        Image playerOne_red("data/playerone_red.bmp", 1800, 300, 300, 100, renderer);
        Image playerTwo_red("data/playertwo_red.bmp", 1800, 400, 300, 100, renderer);
        Image winner("data/winner.bmp", 1800, 400, 300, 100, renderer);
        imagesPlayers.push_back(playerOne);
        imagesPlayers.push_back(playerTwo);
        imagesPlayers.push_back(playerOne_red);
        imagesPlayers.push_back(playerTwo_red);
        imagesPlayers.push_back(winner);
        Image zero("data/0.bmp", 1800, 300, 100, 100, renderer);
        Image one("data/1.bmp", 1800, 300, 100, 100, renderer);
        Image two("data/2.bmp", 1800, 300, 100, 100, renderer);
        Image three("data/3.bmp", 1800, 300, 100, 100, renderer);
        Image four("data/4.bmp", 1800, 300, 100, 100, renderer);
        Image five("data/5.bmp", 1800, 300, 100, 100, renderer);
        Image six("data/6.bmp", 1800, 300, 100, 100, renderer);
        Image seven("data/7.bmp", 1800, 300, 100, 100, renderer);
        Image eight("data/8.bmp", 1800, 300, 100, 100, renderer);
        Image nine("data/9.bmp", 1800, 300, 100, 100, renderer);
        numbers.push_back(zero);
        numbers.push_back(one);
        numbers.push_back(two);
        numbers.push_back(three);
        numbers.push_back(four);
        numbers.push_back(five);
        numbers.push_back(six);
        numbers.push_back(seven);
        numbers.push_back(eight);
        numbers.push_back(nine);
    }


    void
    render(std::array<int, 2> &points, bool &isPlayerOneTurn, int winner, std::shared_ptr<SDL_Renderer> &renderer) {
        if (winner == 0) {
            imagesPlayers[1].render(1800, 400, renderer);
            imagesPlayers[2].render(1800, 300, renderer);
            imagesPlayers[4].render(2150, 295, renderer);
        } else if (winner == 1) {
            imagesPlayers[0].render(1800, 300, renderer);
            imagesPlayers[3].render(1800, 400, renderer);
            imagesPlayers[4].render(2150, 395, renderer);
        } else {
//            imagesPlayers[4].render(2150, 295, renderer);
            if (isPlayerOneTurn) imagesPlayers[2].render(1800, 300, renderer);
            else imagesPlayers[0].render(1800, 300, renderer);
            if (!isPlayerOneTurn) imagesPlayers[3].render(1800, 400, renderer);
            else imagesPlayers[1].render(1800, 400, renderer);
        }
        numbers[points[0]].render(2090, 293, renderer);
        numbers[points[1]].render(2090, 393, renderer);
    }

};

#endif