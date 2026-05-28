#pragma once
#include "Enemy.h"
#include "Texture.h"
#include "Animation.h"

class EntityManager;

class Plant : public Enemy
{
public:
	Plant(Vector2f startPos);
	~Plant() override;

	void SetEntityManager(EntityManager* manager);

	void Update(float elapsedSec) override;
	void Draw() const override;

private:
	void Fire(const Vector2f& playerPos);

	EntityManager* m_pEntityManager{ nullptr };

	static Animation* m_pPlantAnimation;
	static int m_InstanceCount;


	float m_AnimTime{ 0.f };
};

