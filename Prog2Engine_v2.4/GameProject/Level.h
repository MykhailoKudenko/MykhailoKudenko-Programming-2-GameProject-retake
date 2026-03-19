#pragma once
#include <vector>
#include "Texture.h"

class Level
{
public:


	Level(std::vector<std::vector<Vector2f>> vertices, std::vector<Rectf> ladders, const std::string& texturePath );

	const std::vector<std::vector<Vector2f>>& GetVertecies() const;

	void Draw() const;

	const std::vector<Rectf>& GetLadders() const;

private:

	std::vector<std::vector<Vector2f>> m_Vertices;
	std::vector<Rectf> m_Ladders;

	Texture m_Texture;
};

