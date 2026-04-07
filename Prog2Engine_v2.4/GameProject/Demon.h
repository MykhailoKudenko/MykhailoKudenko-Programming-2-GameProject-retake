#pragma once
#include "Animation.h"
#include "Enemy.h"

class EntityManager;

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

class Demon : public Enemy
{
public:
	Demon(Vector2f startPos);

	void SetEntityManager(EntityManager* manager);

	static void InitializeAssets();
	static void FreeAssets();


	void Update(float elapsedSec) override;
	void Draw() const override;

	

private:
	bool SpawnUpdate(float elapsedSec);
	bool MoveToThePoint(float elapsedSec, const Vector2f& TargetPoint);
	bool UpdateParabolaAttack(float elapsedSec, const Vector2f& playerPos, bool movingRight);
	bool UpdateShooting(float elapsedSec, const Vector2f& playerPos);

	void Fire(const Vector2f& playerPos);

	EntityManager* m_pEntityManager{ nullptr };

	Vector2f m_TopRightModificatior{ 100, 80 };


	static Animation* m_pFlyAnimation;
	static Animation* m_pShootAnimation;
	static Animation* m_pSpawnAnimation;
	float m_AnimTime{ 0.f };

	float m_ParabolaProgress{ 0.f };
	float m_ParabolaHorizontalSpeed{ 140.f };

	DemonState m_MyState{ DemonState::Spawning };
	
	bool m_IsAtRightSide{ true };
	bool m_HasFiredThisShot{ false };
};
