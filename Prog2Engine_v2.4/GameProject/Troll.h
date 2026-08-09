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

	bool SpawnUpdate();
	bool UpdateShooting(float elapsedSec, const Vector2f& playerPos);
	bool UpdateWalking(float elapsedSec, const Vector2f& playerPos);
	void Fire(const Vector2f& playerPos);

	void ApplyGravity(float elapsedSec);
	void ApplyHorisontalMovement(float elapsedSec);

	void StartJump();
	bool UpdateJumping( );
	bool UpdateFalling( );

	Animation m_JumpAnimation;
	Animation m_ShootAnimation;
	Animation m_WalkAnimation;
	Animation m_SpawnAnimation;

	EntityManager* m_pEntityManager{ nullptr };
	const std::vector<std::vector<Vector2f>>* m_pVertices{ nullptr };

	TrollState m_MyState{ TrollState::Spawning };

	bool m_HasFiredThisShot{ false };

	float m_WalkTimer{ 0.f };

	bool m_DoJumpNext{ false };
	bool m_IsGrounded{ false };

	const float m_JumpSpeed{ 180.f };

};