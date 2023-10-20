#include "AudioManager.h"


std::map<string, Mix_Chunk*> AudioManager::SoundEffects;
std::map<string, Mix_Music*> AudioManager::Music;

void AudioManager::Init(const char* SoundEffects_Dir, const char* Music_Dir)
{
    SoundEffects = std::map<string, Mix_Chunk*>();

    string path = SoundEffects_Dir;

    for (const auto& file : directory_iterator(path)) {//itterates through files in directory
        if (file.path().extension() == ".wav" || file.path().extension() == ".WAV") {
            string filepath(file.path().u8string());
            string alias(file.path().filename().u8string());
            LoadSoundEffect(filepath.c_str(), alias.c_str());//loads effect
        }
    }


    Music = std::map<string, Mix_Music*>();

    path = Music_Dir;

    for (const auto& file : directory_iterator(path)) {//itterates through files in directory
        if (file.path().extension() == ".wav" || file.path().extension() == ".WAV"||file.path().extension() == ".mp3" || file.path().extension() == ".MP3") {
            string filepath(file.path().u8string());
            string alias(file.path().filename().u8string());
            LoadSoundMusic(filepath.c_str(), alias.c_str());//loads music
        }
    }

}

void AudioManager::LoadSoundEffect(const char* filePath, const char* alias) // loads sound effect via path
{
    Mix_Chunk* sfx = Mix_LoadWAV(filePath);
    if (!sfx) {
        std::cout << "Failed to load Sound Effect : "<< alias << Mix_GetError() << std::endl;
    }
    else
    {
        std::cout << "Loaded Sound :" << alias << std::endl;
        SoundEffects.insert_or_assign(alias, sfx);
    }
}

void AudioManager::LoadSoundMusic(const char* filePath, const char* alias)//loads music via path
{
    Mix_Music* sfx = Mix_LoadMUS(filePath);
    if (!sfx) {
        std::cout << "Failed to load Music : " << alias << Mix_GetError() << std::endl;
    }
    else
    {
        std::cout << "Loaded Music :" << alias << std::endl;
        Music.insert_or_assign(alias, sfx);
    }
}

void AudioManager::PlaySoundEffect(const char* alias,int volume)//plays sound effect
{
    Mix_Volume(1, volume);
    Mix_PlayChannel(1,SoundEffects.at(alias),0);
}

void AudioManager::PlayMusic(const char* alias)//plays music
{
    Mix_PlayMusic(Music.at(alias),-1);
}

void AudioManager::SetMusicVolume(int volume)//sets music volume
{
    Mix_VolumeMusic(volume);
}

void AudioManager::Dispose()
{
    {
        for (const auto& p : SoundEffects)//frees sound effects from memory
        {
            Mix_FreeChunk(p.second);
        }
    }

    {
        for (const auto& p : Music)
        {
            Mix_FreeMusic(p.second);//frees music effects from memory
        }
    }
}
