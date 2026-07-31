#pragma once
#include "Texture.h"
#include <vector>

class TextureManager final
{
public:


	static TextureManager& GetInstance();

	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
	TextureManager(TextureManager&&) = delete;
	TextureManager& operator=(TextureManager&&) = delete;

	const Texture* GetTexture(const std::string& path);

private:

	struct TextureEntry
	{
		std::string path;
		Texture* pTexture;
	};
	
	TextureManager() = default;
	~TextureManager();

	std::vector<TextureEntry*> m_Textures;
};

