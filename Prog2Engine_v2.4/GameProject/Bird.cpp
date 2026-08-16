#include "pch.h"
#include "Bird.h"
#include "utils.h"
#include <cmath>




Bird::Bird(const Vector2f& startPos, bool facingRight)
	: Enemy(Rectf{ startPos.x, startPos.y, 12.f, 13.f }, facingRight ? 25.f : -25.f, facingRight),
	m_MyState{ BirdState::Spawning },
	m_StartY{ startPos.y },
	m_FlyTime{0.f},
	m_Amplitude{8.f},
	m_Frequency{2.f},
	m_FlyAnimation{ "BirdFly.png", 2, 0.15f, true},
	m_SpawnAnimation{ "BirdSpawn.png", 2, 0.52f, false}
{
}

void Bird::Update(float elapsedSec)
{
	if (m_MyState == BirdState::Spawning)
	{
		m_SpawnAnimation.Update(elapsedSec);
		if (m_SpawnAnimation.IsFinished())
		{
			m_MyState = BirdState::Flying;
			m_FlyTime = 0.f;
		}
		return;
	}

	if (m_MyState == BirdState::Flying)
	{
		m_FlyAnimation.Update(elapsedSec);
		m_FlyTime += elapsedSec;

		m_Collider.left += m_Speed * elapsedSec;
		m_Collider.bottom = m_StartY + std::sin(m_FlyTime * m_Frequency) * m_Amplitude;
	}
}

void Bird::Draw() const
{
	if (m_MyState == BirdState::Spawning)
	{
		m_SpawnAnimation.Draw(m_Collider, false, false);
	}
	else if (m_MyState == BirdState::Flying)
	{
		m_FlyAnimation.Draw(m_Collider, m_IsFacingRight);
	}
}

bool Bird::IsSpawning() const
{
	return m_MyState == BirdState::Spawning;
}

