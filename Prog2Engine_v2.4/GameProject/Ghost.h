#pragma once
#include "Enemy.h"
#include "Animation.h"

class EntityManager;



class Ghost final : public Enemy
{
public:
	Ghost(Vector2f startPos, bool facingRight);

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

	void SetEntityManager(EntityManager* manager);

	virtual bool IsSpawning() const override;


private:

	enum class GhostState
	{
		Spawning,
		Flying,
		Dropping
	};


	GhostState m_State;

	float m_DropDistance;
	float m_DropSpeed;

	float m_ExtraPastPlayer;
	bool m_HasPassedPlayerX;

	float m_TargetBottomAfterDrop;

	Animation m_FlyAnimation;
	Animation m_SpawnAnimation;

	EntityManager* m_pEntityManager;
};