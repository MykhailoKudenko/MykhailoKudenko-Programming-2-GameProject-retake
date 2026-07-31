#pragma once
#include "Enemy.h"
#include "Animation.h"

class EntityManager;

enum class GhostState
{
	Spawning,
	Flying,
	Dropping
};

class Ghost : public Enemy
{
public:
	Ghost(Vector2f startPos, bool facingRight);
	~Ghost() override;


	void Update(float elapsedSec) override;
	void Draw() const override;

	void SetEntityManager(EntityManager* manager);

	bool IsSpawning() const override;
private:

	EntityManager* m_pEntityManager{ nullptr };

	GhostState m_State{ GhostState::Spawning };

	float m_AnimTime{ 0.f };

	float m_DropDistance{ 30.f };
	float m_DropSpeed{ 20.f };

	float m_ExtraPastPlayer{ 80.f };
	bool m_HasPassedPlayerX{ false };

	float m_TargetBottomAfterDrop{ 0.f };

	static Animation* m_pFlyAnimation;
	static Animation* m_pSpawnAnimation;
	static int m_InstanceCount;
};