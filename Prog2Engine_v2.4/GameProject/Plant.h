#pragma once
#include "Enemy.h"
#include "Texture.h"
#include "Animation.h"

class EntityManager;

class Plant : public Enemy
{
public:
	Plant(Vector2f startPos);

	void SetEntityManager(EntityManager* manager);

	static void InitializeAssets();
	static void FreeAssets();


	void Update(float elapsedSec) override;
	void Draw() const override;

	

private:
	void Fire(const Vector2f& playerPos);

	EntityManager* m_pEntityManager{ nullptr };

	static Animation* m_pPlantAnimation;
	float m_AnimTime{ 0.f };
};

