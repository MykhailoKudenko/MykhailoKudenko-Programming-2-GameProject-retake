#include "pch.h"
#include "Plant.h"
#include "utils.h"
#include "EntityManager.h"
#include <cmath>


Plant::Plant(const Vector2f& startPos, EntityManager* manager)
	: Enemy(Rectf{ startPos.x, startPos.y, 16, 24 }, 0, false, 100, false, Effect::EffectType::Fire),
	m_PlantAnimation{ "Plant.png", 2, 1.06f, false},
	m_pEntityManager{manager}
{
}

void Plant::Update(float elapsedSec)
{
	m_PlantAnimation.Update(elapsedSec);
	if (m_PlantAnimation.IsFinished())
	{
		if (m_pEntityManager == nullptr)
		{
			return;
		}
		Fire();
		m_PlantAnimation.Reset();
	}
}
void Plant::Fire()
{
	Vector2f spawnPos{
		m_Collider.left + m_Collider.width / 2.f,
		m_Collider.bottom + m_Collider.height / 2.f
	};

	Vector2f direction{
		m_pEntityManager->GetPlayerPosition().x - spawnPos.x,
		m_pEntityManager->GetPlayerPosition().y - spawnPos.y
	};

	m_pEntityManager->SpawnEnemyProjectile(spawnPos, direction.Normalized(), SimpleProjectile::SimpleProjectileType::Plant);
}

void Plant::Draw() const
{
	m_PlantAnimation.Draw(m_Collider, m_IsFacingRight);
}