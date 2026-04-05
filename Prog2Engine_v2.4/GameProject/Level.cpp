#include "pch.h"
#include "Level.h"
#include "utils.h"
#include <algorithm>

Level::Level(
	std::vector<std::vector<Vector2f>> vertices,
	std::vector<Rectf> ladders,
	std::vector<MovingPlatform> platforms,
	std::vector<EnemySpawnPoint> enemySpawnPoints,
	std::vector<EnemySpawnArea> enemySpawnAreas,
	const std::string& PlatformTexturePath,
	const std::string& LevelTexturePath)
	: m_Vertices{ vertices }
	, m_Ladders{ ladders }
	, m_Platforms{ platforms }
	, m_EnemySpawnPoints{ enemySpawnPoints }
	, m_EnemySpawnAreas{ enemySpawnAreas }
	, m_Texture{ LevelTexturePath }
	, m_PlatformTexture{ PlatformTexturePath }
{
}

const std::vector<std::vector<Vector2f>>& Level::GetVertecies() const
{
	return m_Vertices;
}

const std::vector<std::vector<Vector2f>>& Level::GetPlatformTopEdges() const
{
	return m_PlatformTopEdges;
}

const std::vector<Rectf>& Level::GetLadders() const
{
	return m_Ladders;
}

std::vector<Level::EnemySpawnPoint>& Level::GetEnemySpawnPoints()
{
	return m_EnemySpawnPoints;
}

std::vector<Level::EnemySpawnArea>& Level::GetEnemySpawnAreas()
{
	return m_EnemySpawnAreas;
}

float Level::GetWidth() const
{
	float maxX{ 0.f };

	for (const std::vector<Vector2f>& shape : m_Vertices)
	{
		for (const Vector2f& vertex : shape)
		{
			if (vertex.x > maxX)
			{
				maxX = vertex.x;
			}
		}
	}

	for (const MovingPlatform& platform : m_Platforms)
	{
		if (platform.rect.left + platform.rect.width > maxX)
		{
			maxX = platform.rect.left + platform.rect.width;
		}
	}

	for (const EnemySpawnArea& area : m_EnemySpawnAreas)
	{
		if (area.area.left + area.area.width > maxX)
		{
			maxX = area.area.left + area.area.width;
		}
	}

	for (const EnemySpawnPoint& point : m_EnemySpawnPoints)
	{
		if (point.position.x > maxX)
		{
			maxX = point.position.x;
		}
	}

	return maxX;
}

float Level::GetHeight() const
{
	float maxY{ 0.f };

	for (const std::vector<Vector2f>& shape : m_Vertices)
	{
		for (const Vector2f& vertex : shape)
		{
			if (vertex.y > maxY)
			{
				maxY = vertex.y;
			}
		}
	}

	for (const MovingPlatform& platform : m_Platforms)
	{
		if (platform.rect.bottom + platform.rect.height > maxY)
		{
			maxY = platform.rect.bottom + platform.rect.height;
		}
	}

	for (const EnemySpawnArea& area : m_EnemySpawnAreas)
	{
		if (area.area.bottom + area.area.height > maxY)
		{
			maxY = area.area.bottom + area.area.height;
		}
	}

	for (const EnemySpawnPoint& point : m_EnemySpawnPoints)
	{
		if (point.position.y > maxY)
		{
			maxY = point.position.y;
		}
	}

	return maxY;
}

void Level::Draw() const
{
	m_Texture.Draw();

	for (const MovingPlatform& platform : m_Platforms)
	{
		m_PlatformTexture.Draw(platform.rect);
	}

	for (const std::vector<Vector2f>& platform : m_Vertices)
	{
		for (size_t i = 0; i + 1 < platform.size(); ++i)
		{
			utils::SetColor(Color4f{ 1.f, 0.f, 1.f, 1.f });
			utils::DrawLine(platform[i], platform[i + 1]);
		}
	}

	for (const Rectf& ladder : m_Ladders)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 1.f, 1.f });
		utils::DrawRect(ladder);
	}
}

void Level::DrawDebugSpawns() const
{
	// Spawn areas in green
	utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	for (const EnemySpawnArea& spawnArea : m_EnemySpawnAreas)
	{
		utils::DrawRect(spawnArea.area);
	}

	// Spawn points in red
	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	for (const EnemySpawnPoint& spawnPoint : m_EnemySpawnPoints)
	{
		const float size{ 8.f };
		Rectf pointRect
		{
			spawnPoint.position.x - size / 2.f,
			spawnPoint.position.y - size / 2.f,
			size,
			size
		};

		utils::DrawRect(pointRect);
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