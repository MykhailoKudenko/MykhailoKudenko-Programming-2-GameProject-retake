#pragma once
#include "Enemy.h"
#include "Texture.h"
#include "Animation.h"

class EntityManager;

class Plant final : public Enemy
{
public:
	Plant(const Vector2f& startPos,EntityManager* manager);
	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

private:
	void Fire();
	Animation m_PlantAnimation;
	EntityManager* m_pEntityManager;
};

