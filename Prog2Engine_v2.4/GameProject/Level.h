#pragma once
#include <vector>
#include "Texture.h"

class Level
{
public:

	struct MovingPlatform
	{
		Rectf rect;
		float speedX;
		float minX;
		float maxX;
	};

	Level(std::vector<std::vector<Vector2f>> vertices, std::vector<Rectf> ladders, std::vector<MovingPlatform> platforms, const std::string& PlatformTexturePath, const std::string& LevelTexturePath );

	const std::vector<std::vector<Vector2f>>& GetVertecies() const;
	const std::vector<std::vector<Vector2f>>& GetPlatformTopEdges() const;

	void Draw() const;
	void Update( float elapsedSec );

	const std::vector<Rectf>& GetLadders() const;

private:

	std::vector<std::vector<Vector2f>> m_Vertices;
	std::vector<Rectf> m_Ladders;
	std::vector<MovingPlatform> m_Platforms;
	std::vector<std::vector<Vector2f>> m_PlatformTopEdges;

	Texture m_Texture;
	Texture m_PlatformTexture;
	const float m_PlatfromSpeedX{ 20 };
	
};

