#pragma once
#include "Enemy.h"
#include "Animation.h"

class EntityManager;



class Ghost final : public Enemy
{
public:
	Ghost(Vector2f startPos, bool facingRight);

	void Update(float elapsedSec) override;
	void Draw() const override;

	void SetEntityManager(EntityManager* manager);

	bool IsSpawning() const override;


private:

	enum class GhostState
	{
		Spawning,
		Flying,
		Dropping
	};

	EntityManager* m_pEntityManager{ nullptr };

	GhostState m_State{ GhostState::Spawning };

	float m_DropDistance{ 30.f };
	float m_DropSpeed{ 20.f };

	float m_ExtraPastPlayer{ 80.f };
	bool m_HasPassedPlayerX{ false };

	float m_TargetBottomAfterDrop{ 0.f };

	Animation m_FlyAnimation;
	Animation m_SpawnAnimation;
};