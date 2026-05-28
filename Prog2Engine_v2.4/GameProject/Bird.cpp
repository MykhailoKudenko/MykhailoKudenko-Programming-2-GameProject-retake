#include "pch.h"
#include "Bird.h"
#include "utils.h"
#include <cmath>

Animation* Bird::m_pFlyAnimation{ nullptr };
Animation* Bird::m_pSpawnAnimation{ nullptr };
int Bird::m_InstanceCount{ 0 };

Bird::Bird(Vector2f startPos, bool facingRight)
	: Enemy(Rectf{ startPos.x, startPos.y, 12.f, 13.f })
	, m_State{ BirdState::Spawning }
	, m_StartY{ startPos.y }
{

	m_IsFacingRight = facingRight;
	m_Speed = 25.f;

	if (!facingRight)
	{
		m_Speed *= -1.f;
	}

	++m_InstanceCount;

	if (m_pFlyAnimation == nullptr)
	{
		m_pFlyAnimation = new Animation("BirdFly.png", 2, 0.15f, true);
	}

	if (m_pSpawnAnimation == nullptr)
	{
		m_pSpawnAnimation = new Animation("BirdSpawn.png", 2, 0.52f, false);
	}
}

Bird::~Bird()
{
	--m_InstanceCount;

	if (m_InstanceCount <= 0)
	{
		delete m_pFlyAnimation;
		m_pFlyAnimation = nullptr;

		delete m_pSpawnAnimation;
		m_pSpawnAnimation = nullptr;

		m_InstanceCount = 0;
	}
}

void Bird::Update(float elapsedSec)
{
	m_AnimTime += elapsedSec;

	if (m_State == BirdState::Spawning)
	{
		if (m_pSpawnAnimation != nullptr && m_pSpawnAnimation->IsTimeFinished(m_AnimTime))
		{
			m_State = BirdState::Flying;
			m_AnimTime = 0.f;
			m_FlyTime = 0.f;
		}
		return;
	}

	if (m_State == BirdState::Flying)
	{
		m_FlyTime += elapsedSec;

		m_Collider.left += m_Speed * elapsedSec;
		m_Collider.bottom = m_StartY + std::sin(m_FlyTime * m_Frequency) * m_Amplitude;
	}
}

void Bird::Draw() const
{
	if (m_State == BirdState::Spawning)
	{
		if (m_pSpawnAnimation != nullptr)
		{
			m_pSpawnAnimation->DrawAtTime(
				Rectf
				{
					m_Collider.left,
					m_Collider.bottom,
					m_pSpawnAnimation->GetFrameWidth(),
					m_pSpawnAnimation->GetFrameHeight()
				},
				m_AnimTime,
				m_IsFacingRight
			);
		}
	}
	else if (m_State == BirdState::Flying)
	{
		if (m_pFlyAnimation != nullptr)
		{
			m_pFlyAnimation->DrawAtTime(
				Rectf
				{
					m_Collider.left,
					m_Collider.bottom,
					m_pFlyAnimation->GetFrameWidth(),
					m_pFlyAnimation->GetFrameHeight()
				},
				m_AnimTime,
				m_IsFacingRight
			);
		}
	}
}



