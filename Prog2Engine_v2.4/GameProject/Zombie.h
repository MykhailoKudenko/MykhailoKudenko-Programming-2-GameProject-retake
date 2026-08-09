#pragma once
#include "Enemy.h"
#include "Animation.h"



class Zombie final : public Enemy
{

public:
	Zombie(const Vector2f& startPos, bool facingRight);

	void Update(float elapsedSec) override;
	void Draw() const override;

	void SetWorld(const std::vector<std::vector<Vector2f>>* vertices);

	bool IsSpawning() const override;

private:
	enum class ZombieState
	{
		Spawning,
		Walking
	};
	ZombieState m_State;

	Animation m_WalkAnimation;
	Animation m_SpawnAnimation;

	const std::vector<std::vector<Vector2f>>* m_pVertices;
};