#include "TextureManager.h"

std::map<string, SDL_Texture*> TextureManager::Textures;
SDL_Renderer* TextureManager::renderer;


void TextureManager::Init(const char* TexturesFolder, SDL_Renderer* _renderer)
{
    renderer = _renderer;
    Textures = std::map<string, SDL_Texture*>();

    string path = TexturesFolder;

    //creates directory if it dosnt exist, aids debugging if solution hasnt been setup properly
    if(std::filesystem::is_directory(TexturesFolder)==false){
        std::filesystem::create_directory(TexturesFolder);
    }

    for (const auto& file : directory_iterator(path)) {//iterates trhough files in directory
        if (file.path().extension() == ".png" || file.path().extension() == ".PNG") {
            string filepath(file.path().u8string());
            string alias(file.path().filename().u8string());
            LoadTexture(filepath.c_str(), alias.c_str());//loads texture
        }
    }
}

void TextureManager::LoadTexture(const char* filePath, const char* alias)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, filePath);//loads sdl texture
    if (!texture) {
        std::cout << "Failed to load Texture : " << SDL_GetError() << std::endl;
    }
    else
    {
        std::cout << "Loaded Texture :" << alias << std::endl;
        Textures.insert_or_assign(alias,texture);// store texture reference to map
    } 
}

SDL_Texture* TextureManager::GetTexture(const char* alias)
{
    SDL_Texture* texture;
    try
    {
        texture = Textures.at(alias);//gets texture by alias
    }
    catch (const std::exception&)
    {
        std::cout << "Failed to Get Texture under alias : " << alias << std::endl;
        texture = nullptr;
    }

    return texture;
}

void TextureManager::Dispose()//clear memory
{
    for (auto const& x : Textures)
    {
        SDL_DestroyTexture(x.second);//dispose of the texture
    }
}
