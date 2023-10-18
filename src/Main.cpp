#include "SDL_net.h"
#include "SDL_image.h"

#include "MyGame.h"
#include "TextureManager.h"
#include "NetworkCommand.h"

#include "Constants.h"

using namespace std;

const char* IP_NAME = "localhost";
const Uint16 PORT = 55555;

bool is_running = true;

MyGame* game = new MyGame();

static int on_receive(void* socket_ptr) {
    TCPsocket socket = (TCPsocket)socket_ptr;

    const int message_length = 1024;

    char message[message_length];
    int received;

    // TODO: while(), rather than do
    do {
        received = SDLNet_TCP_Recv(socket, message, message_length);
        message[received] = '\0';

        string log=string(message);
        //DEBUG("DataIncome"+ log)


        //find Multiple Commands in 1 Message
        vector<NetworkCommand> Commands= vector<NetworkCommand>();
        string CurrentCommand="";
        string CurrentArg = "";
        vector<string> CurrentArgs = vector<string>();
        bool GotCommand = false;
        for (size_t i = 0; i < received; i++)
        {
            if (message[i] == '|') {

                Commands.push_back(NetworkCommand(CurrentCommand, CurrentArgs));
                CurrentCommand = "";
                CurrentArgs.clear();
                GotCommand = false;
            }
            else {
                if (GotCommand == false) {
                    if (message[i] == ',') {
                        GotCommand = true;
                    }
                    else {
                        CurrentCommand.push_back(message[i]);
                    }
                    
                }
                else if (message[i] == ',') {
                    CurrentArgs.push_back(CurrentArg);
                    CurrentArg.clear();
                }
                else {
                    CurrentArg.push_back(message[i]);
                }
                
            }
        }

        for each (auto item in Commands)
        {
            //DEBUG(item.Command<<" :"<<item.Args.size())
            game->on_receive(item.Command, item.Args);

            if (item.Command == "exit") {
                break;
            }
        }

       

    } while (received > 0 && is_running);

    return 0;
}

static int on_send(void* socket_ptr) {
    TCPsocket socket = (TCPsocket)socket_ptr;

    while (is_running) {
        if (game->messages.size() > 0) {
            string message = "";

            for (auto m : game->messages) {
                message +=m;
            }

            game->messages.clear();

            //cout << "Sending_TCP: " << message << endl;

            SDLNet_TCP_Send(socket, message.c_str(), message.length());
        }

        SDL_Delay(1);
    }

    return 0;
}

void loop(SDL_Renderer* renderer) {
    SDL_Event event;

    while (is_running) {
        // input
        while (SDL_PollEvent(&event)) {
            if ((event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN|| event.type == SDL_MOUSEBUTTONUP || event.type == SDL_KEYUP) && event.key.repeat == 0) {
                game->input(event);

                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        is_running = false;
                        break;

                    default:
                        break;
                }
            }

            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        game->update();

        game->render(renderer);

        SDL_RenderPresent(renderer);

        SDL_Delay(17);
    }
}

int run_game() {
    SDL_Window* window = SDL_CreateWindow(
        "Dungen Crawler",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WindowWidth, WindowHeight,
        SDL_WINDOW_SHOWN
    );

    if (nullptr == window) {
        std::cout << "Failed to create window" << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (nullptr == renderer) {
        std::cout << "Failed to create renderer" << SDL_GetError() << std::endl;
        return -1;
    }

    TextureManager::Init("Textures/", renderer);

    game->Init();

    loop(renderer);

    return 0;
}

int main(int argc, char** argv) {

    // Initialize SDL
    if (SDL_Init(0) == -1) {
        printf("SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }

    // Initialize SDL_net
    if (SDLNet_Init() == -1) {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(2);
    }

    if (!(IMG_Init(IMG_INIT_PNG)))
    {
        std::cout << "Img_Init FAILED TO INIT ERROR: " << IMG_GetError() << std::endl;
        exit(3);
    }

    
    IPaddress ip;

    // Resolve host (ip name + port) into an IPaddress type
    if (SDLNet_ResolveHost(&ip, IP_NAME, PORT) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(4);
    }

    // Open the connection to the server
    TCPsocket socket = SDLNet_TCP_Open(&ip);

    if (!socket) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(5);
    }

    SDL_CreateThread(on_receive, "ConnectionReceiveThread", (void*)socket);
    SDL_CreateThread(on_send, "ConnectionSendThread", (void*)socket);

    run_game();

    game->Dispose();
    delete game;

    //clean up textures
    TextureManager::Dispose();

    // Close connection to the server
    SDLNet_TCP_Close(socket);

    // Shutdown SDL_net
    SDLNet_Quit();

    // Shutdown SDL
    SDL_Quit();



    return 0;
}