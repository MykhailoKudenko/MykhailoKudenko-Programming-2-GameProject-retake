#pragma once
#include "Animation.h"
#include "Enemy.h"

class EntityManager;

enum class TrollState
{
	Spawning,
	Shooting,
	Walking,
	Jumping,
	Falling
};

class Troll final : public Enemy
{
public:
	Troll(Vector2f startPos);
	~Troll() override;

	void Update(float elapsedSec) override;
	void Draw() const override;


	void SetEntityManager(EntityManager* manager);
	void SetWorld(const std::vector<std::vector<Vector2f>>* vertices);
	
		
	
private:
	bool SpawnUpdate(float elapsedSec);
	bool UpdateShooting(float elapsedSec, const Vector2f& playerPos);
	bool UpdateWalking(float elapsedSec, const Vector2f& playerPos);
	void Fire(const Vector2f& playerPos);

	void ApplyGravity(float elapsedSec);
	void ApplyHorisontalMovement(float elapsedSec);

	void StartJump();
	bool UpdateJumping( );
	bool UpdateFalling( );

	static Animation* m_pJumpAnimation;
	static Animation* m_pShootAnimation;
	static Animation* m_pWalkAnimation;
	static Animation* m_pSpawnAnimation;
	static int m_InstanceCount;

	EntityManager* m_pEntityManager{ nullptr };
	const std::vector<std::vector<Vector2f>>* m_pVertices{ nullptr };

	TrollState m_MyState{ TrollState::Spawning };

	bool m_HasFiredThisShot{ false };

	float m_AnimTime{ 0.f };
	float m_WalkTimer{ 0.f };

	const float m_Gravity{ -250.f }; 

	bool m_DoJumpNext{ false };
	bool m_IsGrounded{ false };

	const float m_JumpSpeed{ 180.f };

};