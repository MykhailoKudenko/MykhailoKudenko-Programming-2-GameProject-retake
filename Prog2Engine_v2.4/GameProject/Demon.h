#pragma once
#include "Animation.h"
#include "Enemy.h"

class EntityManager;



class Demon final : public Enemy
{
public:
	Demon(const Vector2f& startPos, EntityManager* manager);

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

private:

	enum class DemonState
	{
		Spawning,
		MoveToTopRight,
		Shooting,
		ParabolaAttack,
		MovingUp,
		MovingDown
	};

	bool MoveToThePoint(float elapsedSec, const Vector2f& targetPoint);
	bool UpdateParabolaAttack(float elapsedSec, const Vector2f& playerPos, bool movingRight);
	bool UpdateShooting(float elapsedSec, const Vector2f& playerPos);

	void Fire();



	Animation m_FlyAnimation;
	Animation m_ShootAnimation;
	Animation m_SpawnAnimation;

	Vector2f m_TopRightModifier;


	float m_ParabolaProgress;
	float m_ParabolaHorizontalSpeed;

	Vector2f m_ParabolaLockedPlayerPos;
	bool m_HasLockedParabolaTarget;

	DemonState m_MyState;
	
	bool m_IsAtRightSide;
	bool m_HasFiredThisShot;

	EntityManager* m_pEntityManager;

};
