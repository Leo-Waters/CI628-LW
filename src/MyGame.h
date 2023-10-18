#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"
#include "Constants.h"
#include "player.h"
#include "Enemy.h"
#include "Level.h"
class MyGame {

    private:
        Player* Players[4];
        Enemy* Enemys[MaxEnemyCount];
        Level* level;
        int LocalPlayerID = -1;
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