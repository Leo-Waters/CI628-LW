#pragma once
#include "SDL_mixer.h"

#include <map>
#include <iostream>
#include <string>
#include <filesystem>

using std::cout; using std::cin;
using std::endl; using std::string;
using std::filesystem::directory_iterator;

//leo waters
//Re used form my CI516 Game AI Project
static class AudioManager
{

private:
	static std::map<string, Mix_Chunk*> SoundEffects;
	static std::map<string, Mix_Music*> Music;
public:
	static void Init(const char* SoundEffects_Dir, const char* Music_Dir);

	static void LoadSoundEffect(const char* filePath, const char* alias);
	static void LoadSoundMusic(const char* filePath, const char* alias);

	static void PlaySoundEffect(const char* alias,int volume);
	static void PlayMusic(const char* alias);

	static void SetMusicVolume(int volume);

	static void Dispose();
};