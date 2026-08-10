#pragma once
#include "Animation.h"
#include "Enemy.h"

class EntityManager;



class Troll final : public Enemy
{
public:
	Troll(Vector2f startPos, const std::vector<std::vector<Vector2f>>* vertices, EntityManager* manager);

	void Update(float elapsedSec) override;
	void Draw() const override;	
private:

	enum class TrollState
	{
		Spawning,
		Shooting,
		Walking,
		Jumping,
		Falling
	};

	void Fire(const Vector2f& playerPos);

	void ApplyVerticalmovement(float elapsedSec);
	void ApplyHorisontalMovement(float elapsedSec);

	Animation m_JumpAnimation;
	Animation m_ShootAnimation;
	Animation m_WalkAnimation;
	Animation m_SpawnAnimation;

	EntityManager* m_pEntityManager;
	const std::vector<std::vector<Vector2f>>* m_pVertices;

	TrollState m_MyState;

	const float m_WalkTimerMax;
	float m_WalkTimerCurrent;

	bool m_DoJumpNext;
	bool m_IsGrounded;

	const float m_JumpTimeMax;
	float m_JumpTimCurrent;
	const float m_JumpSpeed;

};