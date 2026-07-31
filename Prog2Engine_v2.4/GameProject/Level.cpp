#include "pch.h"
#include "Level.h"
#include "utils.h"
#include <algorithm>
#include "SvgParser.h"
#include <iostream>

Level::Level(
	std::vector<std::vector<Vector2f>> vertices,
	std::vector<std::vector<Vector2f>> playerOnlyVertices,
	std::vector<Rectf> ladders,
	std::vector<MovingPlatform> platforms,
	std::vector<EnemySpawnPoint> enemySpawnPoints,
	std::vector<EnemySpawnArea> enemySpawnAreas,
	std::vector<DropSpawnPoint> dropSpawnPoints,
	const std::string& platformTexturePath,
	const std::string& levelTexturePath)
	: m_Vertices{ vertices }
	, m_PlayerOnlyVertices{ playerOnlyVertices }
	, m_Ladders{ ladders }
	, m_Platforms{ platforms }
	, m_EnemySpawnPoints{ enemySpawnPoints }
	, m_EnemySpawnAreas{ enemySpawnAreas }
	, m_DropSpawnPoints{ dropSpawnPoints }
	, m_Texture{ levelTexturePath }
	, m_PlatformTexture{ platformTexturePath }
{
}


Level::Level(const std::string& svgPath,
	std::vector<std::vector<Vector2f>> playerOnlyVertices,
	std::vector<Rectf> ladders,
	std::vector<MovingPlatform> platforms,
	std::vector<EnemySpawnPoint> enemySpawnPoints,
	std::vector<EnemySpawnArea> enemySpawnAreas,
	std::vector<DropSpawnPoint> dropSpawnPoints,
	const std::string& platformTexturePath,
	const std::string& levelTexturePath)
	: m_PlayerOnlyVertices{ playerOnlyVertices }
	, m_Ladders{ ladders }
	, m_Platforms{ platforms }
	, m_EnemySpawnPoints{ enemySpawnPoints }
	, m_EnemySpawnAreas{ enemySpawnAreas }
	, m_DropSpawnPoints{ dropSpawnPoints }
	, m_Texture{ levelTexturePath }
	, m_PlatformTexture{ platformTexturePath }
{
	LoadFromSvg(svgPath);
}

void Level::LoadFromSvg(const std::string& svgPath)
{
	m_Vertices.clear();

	if (!SVGParser::GetVerticesFromSvgFile(svgPath, m_Vertices))
	{
		std::cout << "Failed SVG " << svgPath << std::endl;
	}
}

const std::vector<std::vector<Vector2f>>& Level::GetVertices() const
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
 std::vector<Level::DropSpawnPoint>& Level::GetDropSpawnPoints()
{
	return m_DropSpawnPoints;
}
 std::vector<std::vector<Vector2f>>& Level::GetPlayerOnlyVertices()
{
	return m_PlayerOnlyVertices;
}

float Level::GetWidth() const
{
	return m_Texture.GetWidth();
}

float Level::GetHeight() const
{
	return m_Texture.GetHeight();
}

void Level::Draw(bool isDebug) const
{
	m_Texture.Draw();

	for (const MovingPlatform& platform : m_Platforms)
	{
		m_PlatformTexture.Draw(platform.rect);
	}

	if (isDebug)
	{
		for (const std::vector<Vector2f>& platform : m_Vertices)
		{
			for (size_t i = 0; i + 1 < platform.size(); ++i)
			{
				utils::SetColor(Color4f{ 1.f, 0.f, 1.f, 1.f });
				utils::DrawLine(platform[i], platform[i + 1]);
			}
		}

		for (const std::vector<Vector2f>& platform : m_PlayerOnlyVertices)
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

