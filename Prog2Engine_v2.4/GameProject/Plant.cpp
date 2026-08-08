#include "pch.h"
#include "Plant.h"
#include "utils.h"
#include "EntityManager.h"
#include <cmath>


Plant::Plant(Vector2f startPos)
	: Enemy(Rectf{ startPos.x, startPos.y, 16, 24 }),
	m_PlantAnimation{ Animation("Plant.png", 2, 1.06f, false) }

{
	m_Speed = 0.f;

	m_EffectType = Effect::EffectType::Fire;
}

void Plant::Update(float elapsedSec)
{
	m_PlantAnimation.Update(elapsedSec);
	if (m_PlantAnimation.IsFinished())
	{
		Fire(m_pEntityManager->GetPlayerPosition());
		m_PlantAnimation.Reset();
	}
}
void Plant::Fire(const Vector2f& playerPos)
{
	if (m_pEntityManager == nullptr)
	{
		return;
	}

	Vector2f spawnPos{
		m_Collider.left + m_Collider.width / 2.f,
		m_Collider.bottom + m_Collider.height / 2.f
	};

	Vector2f direction{
		playerPos.x - spawnPos.x,
		playerPos.y - spawnPos.y
	};

	m_pEntityManager->SpawnEnemyProjectile(spawnPos, direction.Normalized(), SimpleProjectile::SimpleProjectileType::Plant);
}

void Plant::Draw() const
{
	m_PlantAnimation.Draw(m_Collider, m_IsFacingRight);
}

void Plant::SetEntityManager(EntityManager* pEntityManager)
{
	m_pEntityManager = pEntityManager;
}
