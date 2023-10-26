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


    string OverFlow=string();

    // TODO: while(), rather than do
    do {
        received = SDLNet_TCP_Recv(socket, message, message_length-1);
        message[received] = '\0';

        //DEBUG("OVERLENG" << OverFlow.length());
        //how big is the data with overflow
        int DataAmount = received + OverFlow.length();

        //create string with both old and new data joined together
        string receivedWithOverFlow(OverFlow);

        for (size_t i = 0; i < received; i++)
        {
            receivedWithOverFlow += message[i];
        }

        //DEBUG("DataIncome"+ receivedWithOverFlow)


        //find Multiple Commands in 1 Message
        vector<NetworkCommand> Commands= vector<NetworkCommand>();
        string CurrentCommand="";
        string CurrentArg = "";
        vector<string> CurrentArgs = vector<string>();
        bool GotCommand = false;

        
        for (size_t i = 0; i < DataAmount; i++)
        {
            if (receivedWithOverFlow[i] == '|') {//Reached end of command add to vector

                Commands.push_back(NetworkCommand(CurrentCommand, CurrentArgs));
                CurrentCommand = "";
                CurrentArgs.clear();
                GotCommand = false;
                OverFlow.clear();
            }
            else {//reading letter by letter
                if (GotCommand == false) {
                    if (receivedWithOverFlow[i] == ','){ //got first arg being the command name
                        GotCommand = true;
                    }
                    else {
                        CurrentCommand.push_back(receivedWithOverFlow[i]);//append letters until , is reached
                    }
                    
                }
                else if (receivedWithOverFlow[i] == ',') {//got arg
                    CurrentArgs.push_back(CurrentArg);//add arg tp list
                    CurrentArg.clear();//clear added
                }
                else {
                    CurrentArg.push_back(receivedWithOverFlow[i]);//add chars
                }
                OverFlow.push_back(receivedWithOverFlow[i]);
                
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


    Uint64 CurrentTime = SDL_GetPerformanceCounter();
    Uint64 LastTime = 0;
   
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

        LastTime = CurrentTime;
        CurrentTime = SDL_GetPerformanceCounter();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        game->update(((CurrentTime - LastTime) * 1000 / (double)SDL_GetPerformanceFrequency()));

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

    TextureManager::Init("Assets/Textures/", renderer);

    

    game->Init(renderer);

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

    // Initialize TTF
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
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