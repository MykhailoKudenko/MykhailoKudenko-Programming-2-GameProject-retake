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

	static void InitializeAssets();
	static void FreeAssets();

	void Update(float elapsedSec) override;
	void Draw() const override;

	void SetEntityManager(EntityManager* manager);

	bool isSpawning() override;
private:

	EntityManager* m_pEntityManager{ nullptr };

	GhostState m_State{ GhostState::Spawning };

	float m_AnimTime{ 0.f };

	float m_DropDistance{ 12.f };
	float m_DropSpeed{ 20.f };

	float m_ExtraPastPlayer{ 80.f };
	bool m_HasPassedPlayerX{ false };

	float m_TargetBottomAfterDrop{ 0.f };

	static Animation* m_pFlyAnimation;
	static Animation* m_pSpawnAnimation;
};