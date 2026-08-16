#pragma once
#include "Enemy.h"
#include "Animation.h"

class EntityManager;



class Ghost final : public Enemy
{
public:
	Ghost(const Vector2f& startPos, bool facingRight, EntityManager* manager);

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

	virtual bool IsSpawning() const override;


private:

	enum class GhostState
	{
		Spawning,
		Flying,
		Dropping
	};


	GhostState m_MyState;

	float m_DropDistance;
	float m_DropSpeed;

	float m_ExtraPastPlayer;
	bool m_HasPassedPlayerX;

	float m_TargetBottomAfterDrop;

	Animation m_FlyAnimation;
	Animation m_SpawnAnimation;

	EntityManager* m_pEntityManager;
};