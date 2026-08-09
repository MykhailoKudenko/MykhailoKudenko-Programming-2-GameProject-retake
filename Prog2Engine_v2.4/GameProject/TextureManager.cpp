#include "pch.h"
#include "TextureManager.h"


TextureManager::TextureManager()
{
}


TextureManager& TextureManager::GetInstance()
{
    static TextureManager instance;
    
    return instance;
}

TextureManager::~TextureManager()
{
    for (const TextureEntry& entry : m_Textures) 
    { 
        delete entry.pTexture; 
    }
    for (const TextureEntry& entry : m_TextTextures) 
    { 
        delete entry.pTexture; 
    }
}


const Texture* TextureManager::GetTexture(const std::string& path)
{
    for (size_t i = 0; i < m_Textures.size(); ++i)
    {
        if (m_Textures[i].path == path)
        {
            return m_Textures[i].pTexture;
        }
    }


    m_Textures.push_back(TextureEntry{ path, new Texture{ path } });
    return m_Textures.back().pTexture;
}

const Texture* TextureManager::GetTextTexture(const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor)
{
    std::string key = text + ':' + fontPath + ':' + std::to_string(ptSize) + ':' + 
        std::to_string(textColor.r) + "," + std::to_string(textColor.g) + "," + std::to_string(textColor.b) + "," + std::to_string(textColor.a);

    for (size_t i = 0; i < m_TextTextures.size(); ++i)
    {
        if (m_TextTextures[i].path == key)
        {
            return m_TextTextures[i].pTexture;
        }
    }

    m_Textures.push_back(TextureEntry{ key, new Texture{ text, fontPath, ptSize, textColor } });
    return m_Textures.back().pTexture;

}