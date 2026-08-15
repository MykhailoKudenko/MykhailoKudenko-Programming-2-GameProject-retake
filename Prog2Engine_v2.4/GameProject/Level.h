#pragma once
#include <vector>
#include "Texture.h"
#include "Drop.h"
class Level final
{
public:

	struct MovingPlatform
	{
		Rectf rect{};
		float speedX{};
		float minX{};
		float maxX{};
	
		void Update(float elapsedSec);
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
		EnemyType type{};
		Vector2f position{};
		bool spawned{ false };
	};

	struct EnemySpawnArea
	{
		EnemyType type{};
		Rectf area{};
		bool spawnAtTheGround{true};
		float timerMax{ 1.f };
		float timer{ 0.f };

		void Update(float elapsedSec);
	};

	struct DropSpawnPoint
	{
		Drop::DropType type{};
		Vector2f position{};
		bool spawned{ false };
	};

	explicit Level(const std::string& txtPath);

	const std::vector<std::vector<Vector2f>>& GetVertices() const;
	const std::vector<std::vector<Vector2f>>& GetPlatformTopEdges() const;
	const std::vector<Rectf>& GetLadders() const;

	const std::vector<EnemySpawnPoint>& GetEnemySpawnPoints() const;
	void MarkEnemySpawnPointSpawned(size_t index);

	const std::vector<EnemySpawnArea>& GetEnemySpawnAreas() const;
	bool IsEnemyAreaReadyToSpawn(size_t index);

	const std::vector<DropSpawnPoint>& GetDropSpawnPoints() const;
	void MarkDropSpawnPointSpawned(size_t index);

	const std::vector<std::vector<Vector2f>>& GetPlayerOnlyVertices() const;
	const Vector2f& GetPlayerPosition() const;

	float GetWidth() const;
	float GetHeight() const;

	void Draw(bool isDebug) const;
	void Update(float elapsedSec);

private:
	void UpdatePlatformTopEdges();
	void LoadFromSvg(const std::string& svgPath);

	std::vector<std::vector<Vector2f>> m_Vertices;
	std::vector<std::vector<Vector2f>> m_PlayerOnlyVertices;

	std::vector<Rectf> m_Ladders;
	std::vector<MovingPlatform> m_Platforms;
	std::vector<std::vector<Vector2f>> m_PlatformTopEdges;

	std::vector<EnemySpawnPoint> m_EnemySpawnPoints;
	std::vector<EnemySpawnArea> m_EnemySpawnAreas;

	std::vector<DropSpawnPoint> m_DropSpawnPoints;

	Vector2f m_PlayerSpawnPos;

	const Texture* m_pTexture;
	const Texture* m_pPlatformTexture;
};