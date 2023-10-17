#include "MyGame.h"

void MyGame::Init()
{
    Players[0] = new Player(0, 100, 200);
    Players[1] = new Player(1, 100, 100);
    Players[2] = new Player(2, 100, 300);
    Players[3] = new Player(3, 100, 400);
    Initalized = true;
}

void MyGame::Dispose()
{
    for (size_t i = 0; i < 4; i++)
    {
        delete Players[i];
    }
}

void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
    if (Initalized) {
        if (cmd == "PLAYER_DATA") {
            
            // we should have exactly 4 arguments
            if (args.size() == 4) {
                
                int ID = stoi(args.at(0));
                Players[ID]->NetworkUpdate(args);
            }
            else
            {
                std::cout << "Didnt Recive All Player Data : " << args.size() << std::endl;
            }
        }
        else {
            std::cout << "Unsure data Received: " << cmd;
            for each (auto var in args)
            {
                std::cout << ","<<var;

            }
            std::cout << std::endl;
        }
    }
}

void MyGame::send(std::string message) {
    messages.push_back(message);
}

void MyGame::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        case SDLK_w:
            send("KEY,"+ std::to_string(PlayerID) +"," + std::string(event.type == SDL_KEYDOWN ? "W_DOWN|" : "W_UP|"));
            break;
        case SDLK_s:
            send("KEY," + std::to_string(PlayerID) + "," + std::string(event.type == SDL_KEYDOWN ? "S_DOWN|" : "S_UP|"));
            break;
        case SDLK_a:
            ("KEY," + std::to_string(PlayerID) + "," + std::string(event.type == SDL_KEYDOWN ? "A_DOWN|" : "A_UP|"));
            break;
        case SDLK_d:
            ("KEY," + std::to_string(PlayerID) + "," + std::string(event.type == SDL_KEYDOWN ? "D_DOWN|" : "D_UP|"));
            break;

    }
}

void MyGame::update() {
    player1.y = game_data.player1Y;
    player2.y = game_data.player2Y;
    Ball.x = game_data.ballX;
    Ball.y = game_data.ballY;
}

void MyGame::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &player1);
    SDL_RenderDrawRect(renderer, &player2);
    SDL_RenderDrawRect(renderer, &Ball);

    for (size_t i = 0; i < 4; i++)
    {
        Players[i]->Render(renderer);
    }
}