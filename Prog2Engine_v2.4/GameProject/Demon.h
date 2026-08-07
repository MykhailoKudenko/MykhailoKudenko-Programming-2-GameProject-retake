#pragma once
#include "Animation.h"
#include "Enemy.h"

class EntityManager;



class Demon final : public Enemy
{
public:
	Demon(Vector2f startPos);

	void SetEntityManager(EntityManager* manager);

	void Update(float elapsedSec) override;
	void Draw() const override;

private:

	enum class DemonState
	{
		Spawning,
		MoveToTopRight,
		Shooting,
		MovingToTheRight,
		MovingToTheLeft,
		MovingUp,
		MovingDown
	};


	bool SpawnUpdate();
	bool MoveToThePoint(float elapsedSec, const Vector2f& targetPoint);
	bool UpdateParabolaAttack(float elapsedSec, const Vector2f& playerPos, bool movingRight);
	bool UpdateShooting(float elapsedSec, const Vector2f& playerPos);

	void Fire(const Vector2f& playerPos);

	EntityManager* m_pEntityManager{ nullptr };

	Vector2f m_TopRightModifier{ 100, 80 };

	Animation m_FlyAnimation;
	Animation m_ShootAnimation;
	Animation m_SpawnAnimation;

	float m_ParabolaProgress{ 0.f };
	float m_ParabolaHorizontalSpeed{ 60.f };

	Vector2f m_ParabolaLockedPlayerPos{};
	bool m_HasLockedParabolaTarget{ false };

	DemonState m_MyState{ DemonState::Spawning };
	
	bool m_IsAtRightSide{ true };
	bool m_HasFiredThisShot{ false };

};
