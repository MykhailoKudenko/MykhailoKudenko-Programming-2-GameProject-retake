#pragma once
#include "Animation.h"
#include "Enemy.h"

class EntityManager;



class Troll final : public Enemy
{
public:
	Troll(Vector2f startPos);

	void Update(float elapsedSec) override;
	void Draw() const override;


	void SetEntityManager(EntityManager* manager);
	void SetWorld(const std::vector<std::vector<Vector2f>>* vertices);
	
		
	
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

	Animation m_pJumpAnimation;
	Animation m_pShootAnimation;
	Animation m_pWalkAnimation;
	Animation m_pSpawnAnimation;

	EntityManager* m_pEntityManager{ nullptr };
	const std::vector<std::vector<Vector2f>>* m_pVertices{ nullptr };

	TrollState m_MyState{ TrollState::Spawning };

	bool m_HasFiredThisShot{ false };

	float m_WalkTimer{ 0.f };

	const float m_Gravity{ -250.f }; 

	bool m_DoJumpNext{ false };
	bool m_IsGrounded{ false };

	const float m_JumpSpeed{ 180.f };

};