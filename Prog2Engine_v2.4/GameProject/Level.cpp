#include "pch.h"
#include "Level.h"
#include "utils.h"
#include "SvgParser.h"
#include <iostream>
#include <fstream>
#include "TextureManager.h"

namespace
{
	Level::EnemyType EnemyTypeFromString(const std::string& type)
	{
		if (type == "Zombie") {return Level::EnemyType::Zombie;}
		if (type == "Bird") {return Level::EnemyType::Bird;}
		if (type == "FlyingKnight") {return Level::EnemyType::FlyingKnight;}
		if (type == "Ghost") {return Level::EnemyType::Ghost;}
		if (type == "Plant") {return Level::EnemyType::Plant;}
		if (type == "Demon") {return Level::EnemyType::Demon;}
		if (type == "Troll") {return Level::EnemyType::Troll;}
		throw std::runtime_error("wrong Enemytype: " + type);
	}

	Drop::DropType PickupTypeFromString(const std::string& type)
	{
		if (type == "Lance") {return  Drop::DropType::Lance;}
		if (type == "Knife") {return  Drop::DropType::Knife;}
		if (type == "Torch") {return  Drop::DropType::Torch;}
		if (type == "Doll") {return  Drop::DropType::Doll;}
		if (type == "MoneyBag") {return  Drop::DropType::MoneyBag;}
		throw std::runtime_error("wrong PickupType: " + type);
	}
}


Level::Level(const std::string& txtPath) : m_pTexture{ nullptr },
m_pPlatformTexture{ nullptr },
m_PlayerSpawnPos{0,0},
m_LevelEndX{ -1.f }
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
		if (line.empty() || line[0] == '#') { continue; }

		std::istringstream iss{ line };
		std::string keyword;
		iss >> keyword;

		if (keyword == "playerStartPos")
		{
			iss >> m_PlayerSpawnPos.x >> m_PlayerSpawnPos.y;
		}
		else if (keyword == "levelEndX")
		{
			iss >> m_LevelEndX;
		}
		else if (keyword == "texture")
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
			esa.spawnAtTheGround = (groundFlag != 0);
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

	if (levelTexturePath.empty())
	{
		throw std::runtime_error("level texture not found ");
	}
	if (m_Platforms.size() > 0 && platformTexturePath.empty())
	{
		throw std::runtime_error("Platfrom texture not found ");
	}
	if (svgPath.empty())
	{
		throw std::runtime_error("SVG not found ");
	}

	m_pTexture = TextureManager::GetInstance().GetTexture(levelTexturePath);
	m_pPlatformTexture = TextureManager::GetInstance().GetTexture(platformTexturePath);
	
	if (m_LevelEndX < 0.f)
	{
		m_LevelEndX = m_pTexture->GetWidth() - 100.f;
	}

	m_PlatformTopEdges.assign(m_Platforms.size(), std::vector<Vector2f>(2));

	LoadFromSvg(svgPath);
}

void Level::LoadFromSvg(const std::string& svgPath)
{
	m_Vertices.clear();

	if (!SVGParser::GetVerticesFromSvgFile(svgPath, m_Vertices))
	{
		throw std::runtime_error("SVG error:  " + svgPath);
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

const std::vector<Level::EnemySpawnPoint>& Level::GetEnemySpawnPoints()const
{
	return m_EnemySpawnPoints;
}
 void Level::MarkEnemySpawnPointSpawned(size_t index)
 {
	 m_EnemySpawnPoints.at(index).spawned = true;
 }

 const std::vector<Level::EnemySpawnArea>& Level::GetEnemySpawnAreas() const
{
	return m_EnemySpawnAreas;
}
 const std::vector<Level::DropSpawnPoint>& Level::GetDropSpawnPoints() const
{
	return m_DropSpawnPoints;
}
 void Level::MarkDropSpawnPointSpawned(size_t index)
 {
	 m_DropSpawnPoints.at(index).spawned = true;
 }

 const std::vector<std::vector<Vector2f>>& Level::GetPlayerOnlyVertices()const
{
	return m_PlayerOnlyVertices;
}

float Level::GetWidth() const
{
	return m_pTexture->GetWidth();
}

float Level::GetHeight() const
{
	return m_pTexture->GetHeight();
}
const Vector2f& Level::GetPlayerPosition() const 
{ 
	return m_PlayerSpawnPos;
}

void Level::Draw(bool isDebug) const
{
	m_pTexture->Draw();

	for (const MovingPlatform& platform : m_Platforms)
	{
		m_pPlatformTexture->Draw(platform.rect);
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
void Level::Update(float elapsedSec)
{
	for (MovingPlatform& platform : m_Platforms)
	{
		platform.Update(elapsedSec);
	}
	for (EnemySpawnArea& spawnArea : m_EnemySpawnAreas)
	{
		spawnArea.Update(elapsedSec);
	}

	UpdatePlatformTopEdges();
}

void Level::MovingPlatform::Update(float elapsedSec)
{
	rect.left += speedX * elapsedSec;

	if (rect.left <= minX)
	{
		rect.left = minX;
		speedX = std::abs(speedX);
	}
	else if (rect.left + rect.width >= maxX)
	{
		rect.left = maxX - rect.width;
		speedX = -std::abs(speedX);
	}
}

void Level::EnemySpawnArea::Update(float elapsedSec)
{
	if (timer < timerMax)
	{
		timer += elapsedSec;
	}
}
bool Level::IsEnemyAreaReadyToSpawn(size_t index)
{
	if (m_EnemySpawnAreas[index].timer >= m_EnemySpawnAreas[index].timerMax)
	{
		m_EnemySpawnAreas[index].timer = 0;
		return true;
	}
	else
	{
		return false;
	}
	
}


void Level::UpdatePlatformTopEdges()
{
	for (size_t i{ 0 }; i < m_Platforms.size(); ++i)
	{
		const Rectf& platformRect{ m_Platforms[i].rect };
		const float platformTop{ platformRect.bottom + platformRect.height };

		std::vector<Vector2f>& edge{ m_PlatformTopEdges[i] };
		edge[0] = Vector2f{ platformRect.left, platformTop };
		edge[1] = Vector2f{ platformRect.left + platformRect.width, platformTop };
	}
}

float Level::GetLevelEndX() const
{
	return m_LevelEndX;
}

