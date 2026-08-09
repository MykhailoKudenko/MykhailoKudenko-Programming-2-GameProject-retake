#include "pch.h"
#include "Demon.h"
#include "EntityManager.h"
#include <cmath>

#include <iostream>
#include <ctime>

Demon::Demon(Vector2f startPos, EntityManager* manager)
	: Enemy(Rectf{ startPos.x, startPos.y, 24, 26 }, 80, false, 200, false, Effect::EffectType::Fire, SoundManager::SFX::None, true, 5),
	m_FlyAnimation{ Animation("DemonFly.png", 2, 0.13f, true) },
	m_ShootAnimation{ Animation("DemonShoot.png", 1, 0.52f, false) },
	m_SpawnAnimation{ Animation("DemonSpawn.png", 3, 0.26f, false) },
	m_TopRightModifier{ 100.f, 80.f },
	m_ParabolaProgress{ 0.f },
	m_ParabolaHorizontalSpeed{ 60.f },
	m_ParabolaLockedPlayerPos{ 0.f, 0.f},
	m_HasLockedParabolaTarget{ false },
	m_MyState{ DemonState::Spawning },
	m_IsAtRightSide{ true },
	m_HasFiredThisShot{ false },
	m_pEntityManager{ manager }
{
}

void Demon::Update(float elapsedSec)
{

	switch (m_MyState)
	{
	case DemonState::Spawning:
		m_SpawnAnimation.Update(elapsedSec);
		if (SpawnUpdate())
		{
			m_MyState = DemonState::MoveToTopRight;
		}
		break;
	case DemonState::MoveToTopRight:
		m_FlyAnimation.Update(elapsedSec);

		if (MoveToThePoint(elapsedSec, m_pEntityManager->GetPlayerPosition() + m_TopRightModifier))
		{
			m_MyState = DemonState::Shooting;
			m_HasFiredThisShot = false;
		}
		break;
	case DemonState::Shooting:
		m_ShootAnimation.Update(elapsedSec);

		if (UpdateShooting(elapsedSec, m_pEntityManager->GetPlayerPosition()))
		{
			int choice = std::rand() % 2;

			m_HasLockedParabolaTarget = false;

			if (choice == 0)
			{
				if (m_IsAtRightSide)
				{
					m_MyState = DemonState::MovingToTheLeft;

				}
				else
				{
					m_MyState = DemonState::MovingToTheRight;

				}
			}
			else
			{
				m_MyState = DemonState::MovingDown;
			}
		}
		break;
	case DemonState::MovingToTheRight:
		m_FlyAnimation.Update(elapsedSec);

		if (UpdateParabolaAttack(elapsedSec, m_pEntityManager->GetPlayerPosition(), true))
		{
			m_IsAtRightSide = true;
			m_MyState = DemonState::Shooting;
			m_HasFiredThisShot = false;
		}
		break;
	case DemonState::MovingToTheLeft:
		m_FlyAnimation.Update(elapsedSec);

		if (UpdateParabolaAttack(elapsedSec, m_pEntityManager->GetPlayerPosition(), false))
		{
			m_IsAtRightSide = false;
			m_MyState = DemonState::Shooting;
			m_HasFiredThisShot = false;
		}
		break;
	case DemonState::MovingUp:
		m_FlyAnimation.Update(elapsedSec);

		if (MoveToThePoint(elapsedSec, Vector2f{ m_Collider.left, m_pEntityManager->GetPlayerPosition().y + m_TopRightModifier.y }))
		{
			m_MyState = DemonState::Shooting;
			m_HasFiredThisShot = false;
		}
		break;
	case DemonState::MovingDown:
		m_FlyAnimation.Update(elapsedSec);

		if (MoveToThePoint(elapsedSec, Vector2f{ m_Collider.left, m_pEntityManager->GetPlayerPosition().y }))
		{
			m_MyState = DemonState::MovingUp;
		}
		break;
	default:
		break;
	}


	
}
void Demon::Fire(const Vector2f& playerPos)
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



	m_pEntityManager->SpawnEnemyProjectile(spawnPos, direction.Normalized(), SimpleProjectile::SimpleProjectileType::Demon);
}




void Demon::Draw() const
{
	switch (m_MyState)
	{
	case DemonState::Spawning:
		m_SpawnAnimation.Draw(m_Collider, false, false);
		break;

	case DemonState::Shooting:
		m_ShootAnimation.Draw(m_Collider, m_IsFacingRight);
		break;

	case DemonState::MoveToTopRight:
	case DemonState::MovingToTheRight:
	case DemonState::MovingToTheLeft:
	case DemonState::MovingUp:
	case DemonState::MovingDown:
		m_FlyAnimation.Draw(m_Collider, m_IsFacingRight);
		break;

	default:
		break;
	}
}

bool Demon::SpawnUpdate()
{
	return m_SpawnAnimation.IsFinished();
}

bool Demon::MoveToThePoint(float elapsedSec, const Vector2f& targetPoint)
{
	Vector2f currentPos{ m_Collider.left, m_Collider.bottom };

	Vector2f direction
	{
		targetPoint.x - currentPos.x,
		targetPoint.y - currentPos.y
	};



	float moveDistance = m_Speed * elapsedSec;

	if (moveDistance >= direction.Length())
	{
		m_Collider.left = targetPoint.x;
		m_Collider.bottom = targetPoint.y;
		return true;
	}

	m_Collider.left += direction.Normalized().x * moveDistance;
	m_Collider.bottom += direction.Normalized().y * moveDistance;

	return false;
}

bool Demon::UpdateParabolaAttack(float elapsedSec, const Vector2f& playerPos, bool movingRight)
{
	if (!m_HasLockedParabolaTarget)
	{
		m_ParabolaLockedPlayerPos = playerPos;
		m_HasLockedParabolaTarget = true;
	}

	Vector2f lockedPlayerPos = m_ParabolaLockedPlayerPos;
	//Bounds

	float leftX = lockedPlayerPos.x - m_TopRightModifier.x;
	float rightX = lockedPlayerPos.x + m_TopRightModifier.x;

	const float moveDir{ movingRight ? 1.f : -1.f };

	m_Collider.left += m_ParabolaHorizontalSpeed * elapsedSec * moveDir;

	if (movingRight && m_Collider.left > rightX)
	{
		m_Collider.left = rightX;
	}
	else if (!movingRight && m_Collider.left < leftX)
	{
		m_Collider.left = leftX;
	}
	//Right bound to left distance
	float width = rightX - leftX;
	
	//protect frim 0 division
	if (std::abs(width) < 0.001f)
	{
		width = 1.f;
	}

	//make t as 0-1 progress indicator
	float t = (m_Collider.left - leftX) / width;

	// Clamp t to valid range
	if (t < 0.f)
	{
		t = 0.f;
	}
	if (t > 1.f)
	{
		t = 1.f;
	}
	//top heigh
	float topY = lockedPlayerPos.y + m_TopRightModifier.y;

	// Height at the middle (when it should hit player)
	float middleTargetY = lockedPlayerPos.y + -20.f;

	// How far down the curve is
	float depth = topY - middleTargetY;

	// Parabola:
	float parabola = 4.f * t * (1.f - t);

	//finding positin
	m_Collider.bottom = topY - depth * parabola;

	// if at the end
	if (movingRight)
	{
		return m_Collider.left >= rightX;
	}
	else
	{
		return m_Collider.left <= leftX;
	}
}

bool Demon::UpdateShooting(float elapsedSec, const Vector2f& playerPos)
{
	Vector2f targetPoint{};

	if (m_IsAtRightSide)
	{
		targetPoint = playerPos + m_TopRightModifier;
	}
	else
	{
		targetPoint = Vector2f{
			playerPos.x - m_TopRightModifier.x,
			playerPos.y + m_TopRightModifier.y
		};
	}

	bool isAtShootPoint = MoveToThePoint(elapsedSec, targetPoint);

	if (!m_HasFiredThisShot)
	{
		Fire(playerPos);
		m_HasFiredThisShot = true;
	}

	
	if (m_ShootAnimation.IsFinished() && isAtShootPoint)
	{
		m_HasFiredThisShot = false;
		return true;
	}

	return false;
}