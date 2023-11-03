#include "SDL_net.h"
#include "SDL_image.h"

#include "MyGame.h"
#include "TextureManager.h"
#include "NetworkCommand.h"
#include "Compression.h"
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


    string CurrentMessage = "";
    string PreviousMessageOverflow = "";

    // TODO: while(), rather than do
    do {
        received = SDLNet_TCP_Recv(socket, message, message_length-1);
        message[received] = '\0';


        int DataAmount = PreviousMessageOverflow.length() + received;
        string receivedWithOverFlow(PreviousMessageOverflow);

        for (size_t i = 0; i < received; i++)
        {
            receivedWithOverFlow += message[i];
        }

        //DEBUG("\nPrev Overflow= "+ PreviousMessageOverflow+"\n")
        //DEBUG("\Recived + Overflow= " + receivedWithOverFlow + "\n")
        CurrentMessage = "";
        bool GotCurrentMessage = false;
        for (size_t i = 0; i < DataAmount; i++)
        {
            if (receivedWithOverFlow[i] == '%') {//reached end of current message
                GotCurrentMessage = true;
                PreviousMessageOverflow = "";
            }
            else if(GotCurrentMessage)
            {
                //got message all next data is for the next message
                PreviousMessageOverflow += receivedWithOverFlow[i];
            }
            else
            {
                //getting the current message
                CurrentMessage += receivedWithOverFlow[i];
            }
            
        }
        
        //has got the full message
        if (GotCurrentMessage) {
            //DEBUG("Incoming" << CurrentMessage);

           

            auto Commands=Compresion::Decompress(CurrentMessage);
            
            for each (auto item in Commands)
            {
                //DEBUG(item->Command<<" :"<<std::to_string(item->Args.size()))
                game->on_receive(item->Command, item->Args);

                if (item->Command == "exit") {
                    delete item;
                    break;
                }
                else
                {
                    delete item;
                }

            }
        }
        else
        {
            //the whole read message is incomplete adding to overflow to be completed with next network data
            PreviousMessageOverflow += CurrentMessage;
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
                //DEBUG("mess "<<m);
                message +=m;
            }

            game->messages.clear();

            auto comp = Compresion::Compress(message);
            comp += "%";
            //DEBUG("comp combined mess " << comp);
            //cout << "Sending_TCP: " << message << endl;

            SDLNet_TCP_Send(socket, comp.c_str(), comp.length());
        }

        SDL_Delay(1);
    }

    return 0;
}

void loop(SDL_Renderer* renderer) {
    SDL_Event event;

    const int FrameRateFPS = 60;
    const int FrameDelay = 1000/ FrameRateFPS;

    Uint32 FrameStart;

   
    int FrameCount = 0;
    float FrameRateTime = 0;


    Uint32 Ticks= SDL_GetTicks();
   
    while (is_running) {

        FrameStart = SDL_GetTicks();

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

        float deltaTime = (SDL_GetTicks() - Ticks) / 1000.0f;

        
        game->update(deltaTime);

        game->render(renderer);

        Ticks = SDL_GetTicks();
        FrameCount++;
        if (FrameRateTime >= 1) {
            DEBUG("Current FPS: " << FrameCount);
            FrameCount = 0;
            FrameRateTime = 0;
        }
        else {
            FrameRateTime += deltaTime;
        }



        SDL_RenderPresent(renderer);

        int FrameTime = SDL_GetTicks() - FrameStart;

        if (FrameDelay > FrameTime) {
            SDL_Delay(FrameDelay - FrameTime);
        }

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

    //test
    Compresion::Test();

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