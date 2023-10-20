#include "Level.h"


Level::Level()
{

}

void Level::Init()
{
	Wall = TextureManager::GetTexture("wall.png");
	Floor = TextureManager::GetTexture("floor.png");
}

void Level::LevelUpdate(string cmd,std::vector<std::string>& args)
{
	if (cmd == "LEVELUPDATE") {//notify Level Update and prepare array for storing the data

		//clean up old level data from memory
		if (leveldata != nullptr) {
			for (size_t i = 0; i < width*height; i++)
			{
				delete &leveldata[i];
			}
			leveldata = nullptr;
		}
		HasRecivedMapData = false;

		//get level size and initalize array to store info
		CurrentLevel = "Level: "+args.at(0);
		width = std::stoi(args.at(1));
		height = std::stoi(args.at(2));;
		leveldata = new int[width * height];
		DEBUG("LEVEL UPDATE STARTED");
		
	}
	else if (cmd == "LEVELDATA") {


		if (args.size() != 1 + width) {
			DEBUG("LEVEL DATA WAS INCOMPLETE "<< args.size());
			return;
		}

		int row = std::stoi(args.at(0));
		//DEBUG("Row "<<row);
		for (int x = 0; x < width; x++)
		{
			int Offset = 1 + x;
			leveldata[row * width + x] = std::stoi(args.at(Offset));
		}
	}
	else if (cmd == "LEVELUPDATECOMPLETE")
	{
		DEBUG("LEVEL UPDATE COMPLETE");
		HasRecivedMapData = true;

	}
}

void Level::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, Floor, NULL, &BackgroundRect);
	if (HasRecivedMapData) {

		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				SDL_Rect pos = SDL_Rect({ (x * TileSize) - Camera::x, (y * TileSize) - Camera::y, TileSize, TileSize });
				if (leveldata[y * width + x] == 1) {
					SDL_RenderCopy(renderer, Wall, NULL, &pos);
				}
				else{
					SDL_RenderCopy(renderer, Floor, NULL, &pos);
				}
			}
		}
	}
}

Level::~Level()
{
}
