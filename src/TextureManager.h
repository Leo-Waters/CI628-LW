#pragma once
#include <SDL.h>
#include<SDL_image.h>

#include <map>
#include <iostream>
#include <string>
#include <filesystem>


using std::cout; using std::cin;
using std::endl; using std::string;
using std::filesystem::directory_iterator;

//re-use, was writen for my CI516 Game AI project
//if error c++ 2017 standard or higher is needed for std::filesystem
static class TextureManager
{
	
private:
	static std::map<string,SDL_Texture*> Textures;
	static SDL_Renderer* renderer;
public:

	static void Init(const char* TexturesFolder,SDL_Renderer * _renderer);
	
	static void LoadTexture(const char* filePath, const char* alias);

	static SDL_Texture* GetTexture(const char* alias);

	static void Dispose();
};

