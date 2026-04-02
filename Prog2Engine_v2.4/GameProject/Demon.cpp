#include "pch.h"
#include "Demon.h"
#include "EntityManager.h"
#include <cmath>

#include <iostream>
#include <ctime>

Animation* Demon::m_pFlyAnimation{ nullptr };
Animation* Demon::m_pShootAnimation{ nullptr };
Animation* Demon::m_pSpawnAnimation{ nullptr };



Demon::Demon(Vector2f StartPos)
	: Enemy(Rectf{ StartPos.x, StartPos.y, 20, 30 })
{
	m_Speed = 80.f;
	m_health = 10;
}

void Demon::Update(float elapsedSec)
{
	m_AnimTime += elapsedSec;
}

void Demon::Update(float elapsedSec, const Vector2f& playerPos)
{
	m_AnimTime += elapsedSec;

	switch (m_MyState)
	{
	case DemonState::Spawning:
		if (SpawnUpdate(elapsedSec))
		{
			m_MyState = DemonState::MoveToTopRight;
		}
		break;
	case DemonState::MoveToTopRight:
		
		if (MoveToThePoint(elapsedSec, playerPos + m_TopRightModificatior))
		{
			m_MyState = DemonState::Shooting;
			m_AnimTime = 0.f;
			m_HasFiredThisShot = false;
		}
		break;
	case DemonState::Shooting:
		if (UpdateShooting(elapsedSec, playerPos))
		{
			int choice = std::rand() % 2;
			
			if (choice == 0)
			{
				if (m_IsAtRightSide)
					m_MyState = DemonState::MovingToTheLeft;
				else
					m_MyState = DemonState::MovingToTheRight;
			}
			else if (choice == 1)
			{
				m_MyState = DemonState::MovingDown;
			}
		}
		break;
	case DemonState::MovingToTheRight:
		if (UpdateParabolaAttack(elapsedSec, playerPos, true))
		{
			m_IsAtRightSide = true;
			m_MyState = DemonState::Shooting;
			m_AnimTime = 0.f;
			m_HasFiredThisShot = false;
		}
		break;
	case DemonState::MovingToTheLeft:
		if (UpdateParabolaAttack(elapsedSec, playerPos, false))
		{
			m_IsAtRightSide = false;
			m_MyState = DemonState::Shooting;
			m_AnimTime = 0.f;
			m_HasFiredThisShot = false;
		}
		break;
	case DemonState::MovingUp:
		if (MoveToThePoint(elapsedSec, Vector2f{ m_Collider.left, playerPos.y + m_TopRightModificatior.y }))
		{
			m_MyState = DemonState::Shooting;
			m_AnimTime = 0.f;
			m_HasFiredThisShot = false;
		}
		break;
	case DemonState::MovingDown:
		if (MoveToThePoint(elapsedSec, Vector2f{ m_Collider.left, playerPos.y }))
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

	m_pEntityManager->SpawnDemonProjectile(spawnPos, direction);
}




void Demon::Draw() const
{
	switch (m_MyState)
	{
	case DemonState::Spawning:
		if (m_pSpawnAnimation != nullptr)
		{
			m_pSpawnAnimation->DrawAtTime(
				Rectf{
					m_Collider.left,
					m_Collider.bottom,
					m_pSpawnAnimation->GetFrameWidth(),
					m_pSpawnAnimation->GetFrameHeight()
				},
				m_AnimTime,
				false
			);
		}
		break;

	case DemonState::Shooting:
		if (m_pShootAnimation != nullptr)
		{
			m_pShootAnimation->DrawAtTime(
				Rectf{
					m_Collider.left,
					m_Collider.bottom,
					m_pShootAnimation->GetFrameWidth(),
					m_pShootAnimation->GetFrameHeight()
				},
				m_AnimTime,
				false
			);
		}
		break;

	case DemonState::MoveToTopRight:
	case DemonState::MovingToTheRight:
	case DemonState::MovingToTheLeft:
	case DemonState::MovingUp:
	case DemonState::MovingDown:
		if (m_pFlyAnimation != nullptr)
		{
			m_pFlyAnimation->DrawAtTime(
				Rectf{
					m_Collider.left,
					m_Collider.bottom,
					m_pFlyAnimation->GetFrameWidth(),
					m_pFlyAnimation->GetFrameHeight()
				},
				m_AnimTime,
				false
			);
		}
		break;

	default:
		break;
	}
}


void Demon::InitializeAssets()
{
	if (m_pFlyAnimation == nullptr)
	{
		m_pFlyAnimation = new Animation("DemonFly.png", 2, 0.13f, true);
	}
	if (m_pShootAnimation == nullptr)
	{
		m_pShootAnimation = new Animation("DemonShoot.png", 1, 0.52f, false);
	}
	if (m_pSpawnAnimation == nullptr)
	{
		m_pSpawnAnimation = new Animation("DemonSpawn.png", 3, 0.26f, false);
	}
}

void Demon::FreeAssets()
{
	delete m_pFlyAnimation;
	m_pFlyAnimation = nullptr;

	delete m_pShootAnimation;
	m_pShootAnimation = nullptr;

	delete m_pSpawnAnimation;
	m_pSpawnAnimation = nullptr;

}

void Demon::SetEntityManager(EntityManager* manager)
{
	m_pEntityManager = manager;
}

bool Demon::SpawnUpdate(float elapsedSec)
{
	if (m_pSpawnAnimation != nullptr && m_pSpawnAnimation->IsTimeFinished(m_AnimTime))
	{
		m_AnimTime = 0.f;
		return true;
	}
	return false;
}

bool Demon::MoveToThePoint(float elapsedSec, const Vector2f& targetPoint)
{
	Vector2f currentPos{ m_Collider.left, m_Collider.bottom };

	Vector2f direction
	{
		targetPoint.x - currentPos.x,
		targetPoint.y - currentPos.y
	};

	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	if (length <= 1.f)
	{
		m_Collider.left = targetPoint.x;
		m_Collider.bottom = targetPoint.y;
		return true;
	}

	direction.x /= length;
	direction.y /= length;

	float moveDistance = m_Speed * elapsedSec;

	if (moveDistance >= length)
	{
		m_Collider.left = targetPoint.x;
		m_Collider.bottom = targetPoint.y;
		return true;
	}

	m_Collider.left += direction.x * moveDistance;
	m_Collider.bottom += direction.y * moveDistance;

	return false;
}

bool Demon::UpdateParabolaAttack(float elapsedSec, const Vector2f& playerPos, bool movingRight)
{
	float leftX = playerPos.x - m_TopRightModificatior.x;
	float rightX = playerPos.x + m_TopRightModificatior.x;
	//Bounds

	float moveDir;
	if (movingRight)
	{
		moveDir = 1.f;
	}
	else
	{
		moveDir = -1.f;
	}

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
		t = 0.f;
	if (t > 1.f) 
		t = 1.f;

	//top heigh
	float topY = playerPos.y + m_TopRightModificatior.y;

	// Height at the middle (when it should hit player)
	float middleTargetY = playerPos.y + -20.f;

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
		targetPoint = playerPos + m_TopRightModificatior;
	}
	else
	{
		targetPoint = Vector2f
		{
			playerPos.x - m_TopRightModificatior.x,
			playerPos.y + m_TopRightModificatior.y
		};
	}

	MoveToThePoint(elapsedSec, targetPoint);

	if (!m_HasFiredThisShot)
	{
		Fire(playerPos);
		m_HasFiredThisShot = true;
	}

	if (m_pShootAnimation != nullptr && m_pShootAnimation->IsTimeFinished(m_AnimTime))
	{
		m_AnimTime = 0.f;
		m_HasFiredThisShot = false;
		return true;
	}

	return false;
}