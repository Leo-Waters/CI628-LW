#include "Level.h"

Level::Level(const int* _leveldata, int _width, int _height, int _tilesize): leveldata(_leveldata), width(_width), height(_height), tilesize(_tilesize)
{
	Wall = TextureManager::GetTexture("wall.png");
	Floor = TextureManager::GetTexture("floor.png");
}

void Level::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, Floor, NULL, &BackgroundRect);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			SDL_Rect pos = SDL_Rect({(x * tilesize)- Camera::x, (y * tilesize) - Camera::y, tilesize, tilesize});
			if (leveldata[y*width+x] == 1) {
				SDL_RenderCopy(renderer, Wall, NULL, &pos);
			}
			else if (leveldata[y * width + x] == 0) {
				SDL_RenderCopy(renderer, Floor, NULL, &pos);
			}
		}
	}
}

Level::~Level()
{
}
