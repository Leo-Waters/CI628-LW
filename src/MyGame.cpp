#include "MyGame.h"

void MyGame::CycleSpectatorPlayer()
{
    SpectatorID++;

    if (SpectatorID == 4) {
        SpectatorID = 0;
    }
    for (int i = SpectatorID; i < 4; i++)
    {
        if (Players[i]->IsDead() == false) {
            SpectatorID = i;
            return;
        }
    }
    if (Players[SpectatorID]->IsDead()) {
        SpectatorID = -1;
    }
}

//returns the combined kills of all the players
int MyGame::GetCombinedKills()
{
    int count = 0;
    for (size_t i = 0; i < 4; i++)
    {
        count += Players[i]->Kills;
    }

    return count;
}

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

    for (int i = 0; i < MaxSpellCount; i++)
    {
        Spells[i] = new Spell(-1000, -1000);
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

    PlayGUI->Imgs->push_back(Image(Anchor::bottomLeft, 100, -140, 150, 150, "FireSpell_Icon.png"));
    PlayGUI->Imgs->push_back(Image(Anchor::bottomRight, -100, -140, 150, 150, "IceSpell_icon.png"));


    //score gui----------------

    ScoreGUI = new GUI(renderer);
    ScoreGUI->Imgs->push_back(Image(Anchor::midCenter, 0, 0,  1400, 600, "UI_Pannel.png"));

    ScoreGUI->Text->push_back(UI_Text(Anchor::midCenter, 0, -200, 60, "Scores", SDL_Color{ 255,255,255 }, renderer));
    ScoreGUI->Text->push_back(UI_Text(Anchor::midCenter, 0, -100, 45, "Player 1 | Health:100 | KILLS: 0|", SDL_Color{ 255,255,255 }, renderer));
    ScoreGUI->Text->push_back(UI_Text(Anchor::midCenter, 0, -50, 45, "Player 2 | Health:100 | KILLS: 0|", SDL_Color{ 255,255,255 }, renderer));
    ScoreGUI->Text->push_back(UI_Text(Anchor::midCenter, 0, 0, 45, "Player 3 | Health:100 | KILLS: 0|", SDL_Color{ 255,255,255 }, renderer));
    ScoreGUI->Text->push_back(UI_Text(Anchor::midCenter, 0, 50, 45, "Player 4 | Health:100 | KILLS: 0|", SDL_Color{ 255,255,255 }, renderer));

    ScoreGUI->Text->push_back(UI_Text(Anchor::midCenter, 0, 120, 45, "Current score is level 0 with, combined kills of 0", SDL_Color{ 255,255,255 }, renderer));
    ScoreGUI->Text->push_back(UI_Text(Anchor::midCenter, 0, 200, 50, "Server best score is level 0, with combined 0 kills", SDL_Color{ 255,255,255 }, renderer));



    //spectator GUI
    SpectatorGUI = new GUI(renderer);

    SpectatorGUI->Text->push_back(UI_Text(Anchor::topCenter, 0, 50, 60, "Spectating", SDL_Color{ 255,255,255 }, renderer));
    SpectatorGUI->Text->push_back(UI_Text(Anchor::bottomCenter, 0, -50, 60, "Press Space to cycle Players", SDL_Color{ 255,255,255 }, renderer));
    //PlayGUI->Text->push_back(UI_Text(Anchor::bottomLeft, 300, -25, 20, "", SDL_Color{ 255,255,255 }, renderer));
   //auto HealthBar = &PlayGUI->Text->at(0);
    //auto HealthBar = &PlayGUI->Text->at(1);




    Initalized = true;

    send("GAMESTATE|");//request a game state update

}

void MyGame::UpdateScoresMenu(SDL_Renderer* renderer)
{
    for (size_t i = 1; i < 5; i++)
    {
        ScoreGUI->Text->at(i).SetText(Players[i-1]->GetState().c_str(), renderer);
    }

    ScoreGUI->Text->at(5).SetText(std::string("Current score is level "+ level->CurrentLevel +" with, combined kills of "+ std::to_string(GetCombinedKills())).c_str(), renderer);
    ScoreGUI->Text->at(6).SetText(std::string("Server best score is level " +BestLevel + " with, combined kills of " + BestKills).c_str(), renderer);
}



void MyGame::Dispose()
{
    for (int i = 0; i < 4; i++)
    {
        delete Players[i];
    }

    for (int i = 0; i < MaxEnemyCount; i++)
    {
        delete Enemys[i];
    }

    for (int i = 0; i < MaxSpellCount; i++)
    {
        delete Spells[i];
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
            if (args.size() == 7) {
                
                int ID = stoi(args.at(0));
                Players[ID]->NetworkUpdate(args);
            }
            else
            {
                std::cout << "Didnt Recive All Player Data : " << args.size() << std::endl;
            }
        }
        else if (cmd == "ENEMY_DATA")
        {
            // we should have exactly 4 arguments
            if (args.size() == 4) {

                if (args.size() > 0) {
                    int ID = stoi(args.at(0));
                    Enemys[ID]->NetworkUpdate(args);


                    Spells[ID]->NetworkUpdate(cmd, args);
                }
                else
                {
                    std::cout << "Didnt Recive All Enemy Data : " << args.size() << std::endl;
                }
            }
        }
        else if(cmd=="SPELL_START"|| cmd == "SPELL_FINISH")
        {
            if (args.size()>0) {
                int ID = stoi(args.at(0));
                

                Spells[ID]->NetworkUpdate(cmd,args);
            }
            else
            {
                std::cout << "Didnt Recive Spell Data : " << args.size() << std::endl;
            }
        }
        else {
            std::cout << "Unsure data Received: " << cmd;
            for each (auto var in args)
            {
                std::cout << "," << var;

            }
            std::cout << std::endl;
        }
    }
}

void MyGame::send(std::string message) {
    messages.push_back(message);
}

void MyGame::input(SDL_Event& event) {

    if (Players[LocalPlayerID]->IsDead()) {
        switch (event.key.keysym.sym) {
        case SDLK_SPACE:
            if (event.type == SDL_KEYDOWN) {
                send("ACTIVE_SPECTATOR|");//notif the server the spectator is active, this will asign them a new player ID if there is a IDLE player
                CycleSpectatorPlayer();
            }
            break;
        case SDLK_TAB:
            if (event.type == SDL_KEYDOWN) {
                ShowingScore = !ShowingScore;
            }
            break;
        }
        return;
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
        case SDLK_e:
            if (event.type == SDL_KEYDOWN) {
                send("KEY,CAST_ICE|");
            }
            break;
        case SDLK_q:
            if (event.type == SDL_KEYDOWN) {
                send("KEY,CAST_FIRE|");
            }
            break;
        case SDLK_TAB:
            if (event.type == SDL_KEYDOWN) {
                ShowingScore = !ShowingScore;
            }
            break;
        

    }


}

void MyGame::update(float tpf) {


    if (LocalPlayerID != -1 && Players[LocalPlayerID]->IsDead()==false) {

        
        HealthBar->Value = Players[LocalPlayerID]->Health;
        Camera::x = Players[LocalPlayerID]->GetPosX() - WindowWidthOffset;
        Camera::y = Players[LocalPlayerID]->GetPosY() - WindowHeightOffset;

        int Delta_x; int Delta_y;
        int mouse_x, mouse_y;

        SDL_GetMouseState(&mouse_x, &mouse_y);
        mouse_x += Camera::x;
        mouse_y += Camera::y;
        Delta_x = (Players[LocalPlayerID]->GetPosX()) - mouse_x;
        Delta_y = (Players[LocalPlayerID]->GetPosY()) - mouse_y;

        float Angle = (atan2(-Delta_x, Delta_y) * 180.0000) / 3.14159265;

        if (std::abs(Angle - Players[LocalPlayerID]->GetAngle())>1) {//send threash hold to reduce angle updates
            send("ANGLE," + std::to_string(Angle) + ",|");
        }



    }
    else if (SpectatorID != -1) {
        if (Players[SpectatorID]->IsDead()) {
            CycleSpectatorPlayer();
        }
        Camera::x = Players[SpectatorID]->GetPosX() - WindowWidthOffset;
        Camera::y = Players[SpectatorID]->GetPosY() - WindowHeightOffset;
    }

    for (size_t i = 0; i < MaxSpellCount; i++)
    {
        Spells[i]->Update(tpf);
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

        for (size_t i = 0; i < MaxSpellCount; i++)
        {
            Spells[i]->Render(renderer);
        }

        if (!ShowingScore) {
            if (LocalPlayerID != -1 && Players[LocalPlayerID]->IsDead() == false) {

                PlayGUI->RenderUI();
            }
            else
            {
                SpectatorGUI->RenderUI();
            }
        }
        else
        {
            UpdateScoresMenu(renderer);
            ScoreGUI->RenderUI();
        }
        


    }
}


