#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"

#include "player.h"

static struct GameData {
    int player1Y = 0;
    int player2Y = 0;
    int ballX = 0;
    int ballY = 0;
} game_data;

class MyGame {

    private:
        Player* Players[4];

        SDL_Rect player1 = { 200, 0, 20, 60 };
        SDL_Rect player2 = { 600, 0, 20, 60 };
        SDL_Rect Ball = { 0, 0, 20, 20 };

    public:
        std::vector<std::string> messages;
        bool Initalized = false;
        void Init();
        void Dispose();

        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void input(SDL_Event& event);
        void update();
        void render(SDL_Renderer* renderer);
};

#endif