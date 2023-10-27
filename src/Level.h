#include "SDL.h"
#include "Camera.h"
#include "TextureManager.h"
#include "Constants.h"
#include "Macros.h"
#include "UI_Text.h"

class  Level
{
public:
     Level();

     void Init();
     void LevelUpdate(string cmd,std::vector<std::string>& args);

     void Draw(SDL_Renderer* renderer);

    ~ Level();
    string CurrentLevel;
private:
    bool HasRecivedMapData=false;
    
    int* leveldata=nullptr;
    int width, height;
    SDL_Texture* Wall;
    SDL_Texture* Floor;
    SDL_Texture* Spawn;
    SDL_Texture* Hatch;
    SDL_Rect BackgroundRect = SDL_Rect{ 0,0,WindowWidth,WindowHeight };
};
