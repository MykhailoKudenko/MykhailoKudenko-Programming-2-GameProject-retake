#include "pch.h"
#include "Bird.h"
#include "utils.h"
#include <cmath>


Bird::Bird(Vector2f startPos, bool facingRight)
	: Enemy(Rectf{ startPos.x, startPos.y, 12.f, 13.f })
	, m_State{ BirdState::Spawning }
	, m_StartY{ startPos.y },
	m_pFlyAnimation{ Animation("BirdFly.png", 2, 0.15f, true) },
	m_pSpawnAnimation{ Animation("BirdSpawn.png", 2, 0.52f, false) }
{

	m_IsFacingRight = facingRight;
	m_Speed = 25.f;

	if (!facingRight)
	{
		m_Speed *= -1.f;
	}

}

void Bird::Update(float elapsedSec)
{
	if (m_State == BirdState::Spawning)
	{
		m_pSpawnAnimation.Update(elapsedSec);
		if (m_pSpawnAnimation.IsFinished())
		{
			m_State = BirdState::Flying;
			m_FlyTime = 0.f;
		}
		return;
	}

	if (m_State == BirdState::Flying)
	{
		m_pFlyAnimation.Update(elapsedSec);
		m_FlyTime += elapsedSec;

		m_Collider.left += m_Speed * elapsedSec;
		m_Collider.bottom = m_StartY + std::sin(m_FlyTime * m_Frequency) * m_Amplitude;
	}
}

void Bird::Draw() const
{
	if (m_State == BirdState::Spawning)
	{
		m_pSpawnAnimation.Draw(m_Collider, false, false);
	}
	else if (m_State == BirdState::Flying)
	{
		m_pFlyAnimation.Draw(m_Collider, m_IsFacingRight);
	}
}



