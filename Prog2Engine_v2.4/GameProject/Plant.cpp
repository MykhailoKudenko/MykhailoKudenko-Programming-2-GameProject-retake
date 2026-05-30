#include "pch.h"
#include "Plant.h"
#include "utils.h"
#include "EntityManager.h"
#include <cmath>

Animation* Plant::m_pPlantAnimation{ nullptr };
int Plant::m_InstanceCount{ 0 };

Plant::Plant(Vector2f StartPos)
	: Enemy(Rectf{ StartPos.x, StartPos.y, 16, 24 })
{
	++m_InstanceCount;

	m_Speed = 0.f;

	if (m_pPlantAnimation == nullptr)
	{
		m_pPlantAnimation = new Animation("Plant.png", 2, 1.06f, false);
	}
	m_EffectType = Effect::EffectType::Fire;

}

Plant::~Plant()
{
	--m_InstanceCount;

	if (m_InstanceCount <= 0)
	{
		delete m_pPlantAnimation;
		m_pPlantAnimation = nullptr;

		m_InstanceCount = 0;
	}
}

void Plant::Update(float elapsedSec)
{
	m_AnimTime += elapsedSec;

	if (m_pPlantAnimation != nullptr && m_pPlantAnimation->IsTimeFinished(m_AnimTime))
	{
		Fire(m_pEntityManager->GetPlayerPosition());

		m_AnimTime = 0.f;
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

	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	if (length > 0.001f)
	{
		direction.x /= length;
		direction.y /= length;
	}
	else
	{
		direction = Vector2f{ 1.f, 0.f };
	}

	m_pEntityManager->SpawnPlantProjectile(spawnPos, direction);
}




void Plant::Draw() const
{

	if (m_pPlantAnimation != nullptr)
	{
		m_pPlantAnimation->DrawAtTime(
			Rectf{
				m_Collider.left,
				m_Collider.bottom,
				m_pPlantAnimation->GetFrameWidth(),
				m_pPlantAnimation->GetFrameHeight()
			},
			m_AnimTime,
			true
		);
	}
}

void Plant::SetEntityManager(EntityManager* pEntityManager)
{
	m_pEntityManager = pEntityManager;
}
