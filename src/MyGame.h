#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"
#include "Constants.h"
#include "player.h"
#include "Spell.h"
#include "Enemy.h"
#include "Level.h"
#include "AudioManager.h"
#include "GUI.h"
class MyGame {

    private:
        GUI* PlayGUI;
        GUI* SpectatorGUI;
        GUI* ScoreGUI;
        Player* Players[4];
        Spell* Spells[MaxSpellCount];
        Enemy* Enemys[MaxEnemyCount];
        Level* level;
        int LocalPlayerID = -1;
        int SpectatorID = -1;
        bool ShowingScore = false;
        void CycleSpectatorPlayer();
        //ui
        UI_Slider* HealthBar;
        UI_Text* LevelTxt;


        int GetCombinedKills();
        string BestLevel = "0";
        string BestKills = "0";
    public:
        std::vector<std::string> messages;
        bool Initalized = false;
        void Init(SDL_Renderer* renderer);
        void Dispose();

        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void input(SDL_Event& event);
        void update(float tpf);
        void render(SDL_Renderer* renderer);


        void UpdateScoresMenu(SDL_Renderer* renderer);
};

#endif