#pragma once
#include "Enemy.h"
#include "Texture.h"
#include "Animation.h"

class EntityManager;

class Plant final : public Enemy
{
public:
	Plant(Vector2f startPos,EntityManager* manager);
	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

private:
	void Fire(const Vector2f& playerPos);

	EntityManager* m_pEntityManager;

	Animation m_PlantAnimation;
};

