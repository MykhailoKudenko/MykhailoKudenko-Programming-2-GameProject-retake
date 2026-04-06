#pragma once
#include "Enemy.h"
#include "Animation.h"

enum class ZombieState
{
	Spawning,
	Walking
};

class Zombie : public Enemy
{

public:
	Zombie(Vector2f StartPos, bool facingRight);

	static void InitializeAssets();
	static void FreeAssets();

	void Update(float elapsedSec) override;
	void Draw() const override;

	void SetWorld(const std::vector<std::vector<Vector2f>>* vertices);

	bool isSpawning() override;

private:
	const std::vector<std::vector<Vector2f>>* m_pVertices{};
	static constexpr float m_Gravity{ -60.f };


	ZombieState m_State{ ZombieState::Spawning };
	float m_AnimTime{ 0.f };

	static Animation* m_pWalkAnimation;
	static Animation* m_pSpawnAnimation;
};