#include "pch.h"
#include "Level.h"
#include "utils.h"
#include <algorithm>
#include "SvgParser.h"
#include <iostream>
#include <fstream>
#include "TextureManager.h"

namespace
{
	Level::EnemyType EnemyTypeFromString(const std::string& s)
	{
		if (s == "Zombie")       return Level::EnemyType::Zombie;
		if (s == "Bird")         return Level::EnemyType::Bird;
		if (s == "FlyingKnight") return Level::EnemyType::FlyingKnight;
		if (s == "Ghost")        return Level::EnemyType::Ghost;
		if (s == "Plant")        return Level::EnemyType::Plant;
		if (s == "Demon")        return Level::EnemyType::Demon;
		if (s == "Troll")        return Level::EnemyType::Troll;
		throw std::runtime_error("wrong Enemytype: ");
	}

	

	PickupType PickupTypeFromString(const std::string& s)
	{
		if (s == "Lance") return PickupType::Lance;
		if (s == "Knife") return PickupType::Knife;
		if (s == "Torch") return PickupType::Torch;
		if (s == "Doll") return PickupType::Doll;
		if (s == "MoneyBag") return PickupType::MoneyBag;
		throw std::runtime_error("wrong PickupType: ");
	}
}


Level::Level(const std::string& txtPath) : m_Texture{ nullptr }
, m_PlatformTexture{ nullptr }
{
	std::ifstream file{ txtPath };
	if (!file.is_open())
	{
		throw std::runtime_error("Didnt find a file:  " + txtPath);
	}

	std::string levelTexturePath;
	std::string platformTexturePath;
	std::string svgPath;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty() || line[0] == '#') continue;

		std::istringstream iss{ line };
		std::string keyword;
		iss >> keyword;

		if (keyword == "texture")
		{
			iss >> levelTexturePath;
		}
		else if (keyword == "platformTexture")
		{
			iss >> platformTexturePath;
		}
		else if (keyword == "svg")
		{
			iss >> svgPath;
		}
		else if (keyword == "ladder")
		{
			Rectf r;
			iss >> r.left >> r.bottom >> r.width >> r.height;
			m_Ladders.push_back(r);
		}
		else if (keyword == "platform")
		{
			MovingPlatform mp;
			iss >> mp.rect.left >> mp.rect.bottom >> mp.rect.width >> mp.rect.height
				>> mp.speedX >> mp.minX >> mp.maxX;
			m_Platforms.push_back(mp);
		}
		else if (keyword == "enemy")
		{
			std::string typeStr;
			EnemySpawnPoint esp;
			iss >> typeStr >> esp.position.x >> esp.position.y;
			esp.type = EnemyTypeFromString(typeStr);
			m_EnemySpawnPoints.push_back(esp);
		}
		else if (keyword == "spawnarea")
		{
			std::string typeStr;
			EnemySpawnArea esa;
			int groundFlag{};
			iss >> typeStr >> esa.area.left >> esa.area.bottom
				>> esa.area.width >> esa.area.height
				>> groundFlag >> esa.timerMax;
			esa.type = EnemyTypeFromString(typeStr);
			esa.SpawnAtTheGround = (groundFlag != 0);
			m_EnemySpawnAreas.push_back(esa);
		}
		else if (keyword == "drop")
		{
			std::string typeStr;
			DropSpawnPoint dsp;
			iss >> typeStr >> dsp.position.x >> dsp.position.y;
			dsp.type = PickupTypeFromString(typeStr);
			m_DropSpawnPoints.push_back(dsp);
		}
		else if (keyword == "polygon")
		{
			std::vector<Vector2f> polygon;
			std::string vertexLine;
			while (std::getline(file, vertexLine) && vertexLine != "endpolygon")
			{
				std::istringstream vertIss{ vertexLine };
				Vector2f v;
				vertIss >> v.x >> v.y;
				polygon.push_back(v);
			}
			m_PlayerOnlyVertices.push_back(polygon);
		}
		else
		{
			std::cout << "wrong keyword:" << keyword << std::endl;
		}
	}
	m_Texture = TextureManager::GetInstance().GetTexture(levelTexturePath);
	m_PlatformTexture = TextureManager::GetInstance().GetTexture(platformTexturePath);

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
	return m_Texture->GetWidth();
}

float Level::GetHeight() const
{
	return m_Texture->GetHeight();
}

void Level::Draw(bool isDebug) const
{
	m_Texture->Draw();

	for (const MovingPlatform& platform : m_Platforms)
	{
		m_PlatformTexture->Draw(platform.rect);
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

