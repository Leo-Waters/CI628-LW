#include "MyGame.h"

void MyGame::Init(SDL_Renderer* renderer)
{
    Players[0] = new Player(0, 100, 200);
    Players[1] = new Player(1, 100, 100);
    Players[2] = new Player(2, 100, 300);
    Players[3] = new Player(3, 100, 400);

    for (int i = 0; i < MaxEnemyCount; i++)
    {
        Enemys[i] = new Enemy(i, -1000, -1000);
    }


    if (LocalPlayerID != -1) {
        Players[LocalPlayerID]->IsLocalPlayer = true;
    }

    if (level == nullptr) {
        level = new Level();
    }

    level->Init();


    Camera::Min_x = 0;
    Camera::Min_y = 0;

    Camera::Max_x = (40 * TileSize) - WindowWidth;
    Camera::Max_y = (40 * TileSize) - WindowHeight;
    


    PlayGUI = new GUI(renderer);


    PlayGUI->Sliders->push_back(UI_Slider(Anchor::topCenter,0, 50, 800, 80, "HP_Bar.png"));

    HealthBar = &PlayGUI->Sliders->at(0);
   

    PlayGUI->Text->push_back(UI_Text(Anchor::topLeft, 150, 50, 40, "CurrentLevel:", SDL_Color{ 255,255,255 }, renderer));
    LevelTxt = &PlayGUI->Text->at(0);

    //PlayGUI->Text->push_back(UI_Text(Anchor::bottomLeft, 300, -25, 20, "", SDL_Color{ 255,255,255 }, renderer));
   //auto HealthBar = &PlayGUI->Text->at(0);
    //auto HealthBar = &PlayGUI->Text->at(1);



    Initalized = true;
}

void MyGame::Dispose()
{
    for (size_t i = 0; i < 4; i++)
    {
        delete Players[i];
    }

    for (size_t i = 0; i < MaxEnemyCount; i++)
    {
        delete Enemys[i];
    }

    delete PlayGUI;
    PlayGUI = nullptr;
}

void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
    if (cmd == "ID") {

        // we should have exactly 1 arguments
        if (args.size() == 1) {

            int ID = stoi(args.at(0));
            DEBUG("Got LocalPlayerID of: " << ID);
            if (ID == -1) {
                DEBUG("No Avalible players, this client is spectating until someone leaves");
            }
            else
            {
                LocalPlayerID = ID;
                if (Initalized) {
                    Players[ID]->IsLocalPlayer = true;
                }
            }
        }
        else
        {
            std::cout << "Didnt Recive ID correctly: " << args.size() << std::endl;
        }
    }

    if (cmd == "LEVELUPDATE"|| cmd == "LEVELDATA"|| cmd=="LEVELUPDATECOMPLETE") {
        if (level == nullptr) {
            level = new Level();
        }
        level->LevelUpdate(cmd, args);
    }

    if (Initalized) {
        if (cmd == "PLAYER_DATA") {
            
            // we should have exactly 5 arguments
            if (args.size() == 5) {
                
                int ID = stoi(args.at(0));
                Players[ID]->NetworkUpdate(args);
            }
            else
            {
                std::cout << "Didnt Recive All Player Data : " << args.size() << std::endl;
            }
        }
        else if(cmd=="ENEMY_DATA")
        {
            // we should have exactly 4 arguments
            if (args.size() == 4) {

                int ID = stoi(args.at(0));
                Enemys[ID]->NetworkUpdate(args);
            }
            else
            {
                std::cout << "Didnt Recive All Enemy Data : " << args.size() << std::endl;
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

    if (LocalPlayerID == -1) {
        Camera::Update(event);
    }
    switch (event.key.keysym.sym) {
        case SDLK_w:
            send("KEY,"+std::string(event.type == SDL_KEYDOWN ? "W_DOWN|" : "W_UP|"));
            break;
        case SDLK_s:
            send("KEY," +std::string(event.type == SDL_KEYDOWN ? "S_DOWN|" : "S_UP|"));
            break;
        case SDLK_a:
            send("KEY," + std::string(event.type == SDL_KEYDOWN ? "A_DOWN|" : "A_UP|"));
            break;
        case SDLK_d:
            send("KEY," + std::string(event.type == SDL_KEYDOWN ? "D_DOWN|" : "D_UP|"));
            break;

    }
}

void MyGame::update() {
    
    if (LocalPlayerID != -1) {

        
        HealthBar->Value = Players[LocalPlayerID]->Health;
        Camera::x = Players[LocalPlayerID]->GetPosX() - WindowWidthOffset;
        Camera::y = Players[LocalPlayerID]->GetPosY() - WindowHeightOffset;
    }
}

void MyGame::render(SDL_Renderer* renderer) {
    
    if (Initalized) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        level->Draw(renderer);
        LevelTxt->SetText(level->CurrentLevel.c_str(), renderer);
        for (size_t i = 0; i < 4; i++)
        {
            Players[i]->Render(renderer);
        }
        for (size_t i = 0; i < MaxEnemyCount; i++)
        {
            Enemys[i]->Render(renderer);
        }

        if (LocalPlayerID != -1) {

            PlayGUI->RenderUI();

            
        }
    }
}