#include "pch.h"
#include "TextureManager.h"


TextureManager::TextureManager()
{
    m_Textures.reserve(500);
    m_TextTextures.reserve(500);
}


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

    for (size_t i = 0; i < m_TextTextures.size(); ++i)
    {
        delete m_TextTextures[i]->pTexture;
        delete m_TextTextures[i];
    }
    m_TextTextures.clear();
}


const Texture* TextureManager::GetTexture(const std::string& path)
{
    for (size_t i = 0; i < m_Textures.size(); ++i)
    {
        if (m_Textures[i]->path == path)
        {
            return m_Textures[i]->pTexture;
        }
    }


    TextureEntry* entry = new TextureEntry();
    entry->path = path;
    entry->pTexture = new Texture(path);
    m_Textures.push_back(entry);
    return entry->pTexture;
}

const Texture* TextureManager::GetTextTexture(const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor)
{
    std::string key = text + ':' + fontPath + ':' + std::to_string(ptSize) + ':' + 
        std::to_string(textColor.r) + "," + std::to_string(textColor.g) + "," + std::to_string(textColor.b) + "," + std::to_string(textColor.a);

    for (size_t i = 0; i < m_TextTextures.size(); ++i)
    {
        if (m_TextTextures[i]->path == key)
        {
            return m_TextTextures[i]->pTexture;
        }
    }

    TextureEntry* entry = new TextureEntry();
    entry->path = key;
    entry->pTexture = new Texture(text, fontPath, ptSize, textColor);
    m_TextTextures.push_back(entry);
    return entry->pTexture;

}