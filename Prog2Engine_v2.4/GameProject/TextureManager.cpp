#include "pch.h"
#include "TextureManager.h"


TextureManager& TextureManager::GetInstance()
{
    static TextureManager instance;
    return instance;
}

TextureManager::~TextureManager()
{
    for (size_t i = 0; i < m_Textures.size(); ++i)
    {
        delete m_Textures[i]->pTexture;
        delete m_Textures[i];
    }
    m_Textures.clear();
}


const Texture* TextureManager::GetTexture(const std::string& path)
{
    for (int i = 0; i < m_Textures.size(); ++i)
    {
        if (m_Textures[i]->path == path)
            return m_Textures[i]->pTexture;
    }


    TextureEntry* entry = new TextureEntry();
    entry->path = path;
    entry->pTexture = new Texture(path);
    m_Textures.push_back(entry);
    return entry->pTexture;
}

