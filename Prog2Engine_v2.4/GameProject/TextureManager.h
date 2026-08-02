#pragma once
#include "Texture.h"
#include "Animation.h"
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
	const Texture* GetTextTexture(const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor);

private:

	struct TextureEntry
	{
		std::string path;
		Texture* pTexture;
	};

	TextureManager();
	~TextureManager();

	std::vector<TextureEntry*> m_Textures;
	std::vector<TextureEntry*> m_TextTextures;

};

