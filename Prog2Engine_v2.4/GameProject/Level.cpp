#include "pch.h"
#include "Level.h"
#include "utils.h"
Level::Level(std::vector<std::vector<Vector2f>> vertices, std::vector<Rectf> ladders, std::vector<MovingPlatform> platforms, const std::string& PlatformTexturePath, const std::string& LevelTexturePath)
	: m_Vertices{ vertices }, m_Ladders{ ladders }, m_Platforms{ platforms }, m_Texture { LevelTexturePath }, m_PlatformTexture{ PlatformTexturePath }
{
		
}

const std::vector<std::vector<Vector2f>>& Level::GetVertecies() const
{
	return(m_Vertices);
}

const std::vector<std::vector<Vector2f>>& Level::GetPlatformTopEdges() const
{
	return m_PlatformTopEdges;
}

void Level::Draw() const
{
	

	m_Texture.Draw();


	for (const MovingPlatform& Proj : m_Platforms)
	{
		m_PlatformTexture.Draw(Proj.rect);
	}

	for (const std::vector<Vector2f>& platform : m_Vertices)
	{
		for (size_t i = 0; i < platform.size() - 1; ++i)
		{
			utils::SetColor(Color4f{ 1.f, 0.f, 1.f, 1.f });
			utils::DrawLine(platform[i], platform[i + 1]);
		}
	}

	for (Rectf ladder : m_Ladders)
	{
		
		utils::SetColor(Color4f{ 1.f, 0.f, 1.f, 1.f });
		utils::DrawRect(ladder);
		
	}

	
}

void Level::Update(float elapsedSec)
{
	for (MovingPlatform& platform : m_Platforms)
	{
		platform.rect.left += platform.speedX * elapsedSec;

		if (platform.rect.left <= platform.minX)
		{
			platform.rect.left = platform.minX;
			platform.speedX = abs(platform.speedX);
		}
		else if (platform.rect.left + platform.rect.width >= platform.maxX)
		{
			platform.rect.left = platform.maxX - platform.rect.width;
			platform.speedX = -abs(platform.speedX);
		}
	}
	m_PlatformTopEdges.clear();

	for (const MovingPlatform& platform : m_Platforms)
	{
		m_PlatformTopEdges.push_back({
			Vector2f{ platform.rect.left, platform.rect.bottom + platform.rect.height },
			Vector2f{ platform.rect.left + platform.rect.width, platform.rect.bottom + platform.rect.height }
			});
	}
}


const std::vector<Rectf>& Level::GetLadders() const
{
	return m_Ladders;
}