#pragma once
#include <vector>
#include "Texture.h"
#include "Drop.h"
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

	enum class EnemyType
	{
		Zombie,
		Bird,
		FlyingKnight,
		Ghost,
		Plant,
		Demon,
		Troll
	};

	struct EnemySpawnPoint
	{
		EnemyType type;
		Vector2f position;
		bool spawned{ false };
	};

	struct EnemySpawnArea
	{
		EnemyType type;
		Rectf area;
		bool SpawnAtTheGround{true};
		float timerMax{ 1.f };
		float timer{ 0.f };
	
	};

	struct DropSpawnPoint
	{
		PickupType type;
		Vector2f position;
		bool spawned{ false };
	};

	Level(
		std::vector<std::vector<Vector2f>> vertices,
		std::vector<std::vector<Vector2f>> playerOnlyVertices,
		std::vector<Rectf> ladders,
		std::vector<MovingPlatform> platforms,
		std::vector<EnemySpawnPoint> enemySpawnPoints,
		std::vector<EnemySpawnArea> enemySpawnAreas,
		std::vector<DropSpawnPoint> dropSpawnPoints,
		const std::string& PlatformTexturePath,
		const std::string& LevelTexturePath);

	Level(const std::string& svgPath,
		std::vector<std::vector<Vector2f>> playerOnlyVertices,
		std::vector<Rectf> ladders,
		std::vector<MovingPlatform> platforms,
		std::vector<EnemySpawnPoint> enemySpawnPoints,
		std::vector<EnemySpawnArea> enemySpawnAreas,
		std::vector<DropSpawnPoint> dropSpawnPoints,
		const std::string& PlatformTexturePath,
		const std::string& LevelTexturePath);

	//rule of 5
	~Level() = default;
	Level(const Level&) = delete;
	Level& operator=(const Level&) = delete;
	Level(Level&&) = delete;
	Level& operator=(Level&&) = delete;

	const std::vector<std::vector<Vector2f>>& GetVertecies() const;
	const std::vector<std::vector<Vector2f>>& GetPlatformTopEdges() const;
	const std::vector<Rectf>& GetLadders() const;

	std::vector<EnemySpawnPoint>& GetEnemySpawnPoints();
	std::vector<EnemySpawnArea>& GetEnemySpawnAreas();

	std::vector<DropSpawnPoint>& GetDropSpawnPoints();

	std::vector<std::vector<Vector2f>>& GetPlayerOnlyVertices();

	float GetWidth() const;
	float GetHeight() const;

	void Draw(bool isDebug) const;
	void DrawDebugSpawns() const;
	void Update(float elapsedSec);

private:
	void LoadFromSvg(const std::string& svgPath);

	std::vector<std::vector<Vector2f>> m_Vertices;
	std::vector<std::vector<Vector2f>> m_PlayerOnlyVertices;

	std::vector<Rectf> m_Ladders;
	std::vector<MovingPlatform> m_Platforms;
	std::vector<std::vector<Vector2f>> m_PlatformTopEdges;

	std::vector<EnemySpawnPoint> m_EnemySpawnPoints;
	std::vector<EnemySpawnArea> m_EnemySpawnAreas;

	std::vector<DropSpawnPoint> m_DropSpawnPoints;

	Texture m_Texture;
	Texture m_PlatformTexture;
	const float m_PlatfromSpeedX{ 20 };
};