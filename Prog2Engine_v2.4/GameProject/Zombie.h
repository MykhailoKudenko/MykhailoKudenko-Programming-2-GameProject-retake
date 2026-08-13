#pragma once
#include "Enemy.h"
#include "Animation.h"



class Zombie final : public Enemy
{

public:
	Zombie(const Vector2f& startPos, bool facingRight, const std::vector<std::vector<Vector2f>>* vertices);

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;
	virtual bool IsSpawning() const override;

private:
	enum class ZombieState
	{
		Spawning,
		Walking
	};
	ZombieState m_MyState;

	Animation m_WalkAnimation;
	Animation m_SpawnAnimation;

	const std::vector<std::vector<Vector2f>>* m_pVertices;
};