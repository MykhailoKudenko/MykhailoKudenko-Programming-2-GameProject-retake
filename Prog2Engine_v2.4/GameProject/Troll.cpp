#include "pch.h"
#include "Troll.h"
#include "utils.h"
#include "EntityManager.h"
#include <cmath>

#include <iostream>

Troll::Troll(Vector2f startPos)
	: Enemy(Rectf{ startPos.x, startPos.y, 43, 40 }),
	m_pJumpAnimation{ Animation("TrollJump.png", 1, 0.13f, true) },
	m_pShootAnimation{ Animation("TrollShoot.png", 1, 0.52f, false) },
	m_pWalkAnimation{ Animation("TrollWalking.png", 2, 0.13f, true) },
	m_pSpawnAnimation{ Animation("TrollStand.png", 1, 0.52f, false) }
{
	m_Speed = 30.f;
	m_Health = 10;
	m_IsBoss = true;


	m_EffectType = Effect::EffectType::Fire;


}

void Troll::Update(float elapsedSec)
{

	m_Velocity.x = 0.f;

	switch (m_MyState)
	{
	case TrollState::Spawning:
		m_pSpawnAnimation.Update(elapsedSec);

		if (SpawnUpdate())
		{
			m_MyState = TrollState::Shooting;
			m_HasFiredThisShot = false;
		}
		break;

	case TrollState::Shooting:
		m_pShootAnimation.Update(elapsedSec);

		if (UpdateShooting(elapsedSec, m_pEntityManager->GetPlayerPosition()))
		{
			if (m_DoJumpNext)
			{
				StartJump();
				m_DoJumpNext = false;
			}
			else
			{
				m_MyState = TrollState::Walking;
				m_WalkTimer = 0.f;
				m_DoJumpNext = true;
			}
		}
		break;

	case TrollState::Walking:
		m_pWalkAnimation.Update(elapsedSec);
		if (UpdateWalking(elapsedSec, m_pEntityManager->GetPlayerPosition()))
		{
			m_MyState = TrollState::Shooting;
			m_HasFiredThisShot = false;
		}
		break;

	case TrollState::Jumping:
		m_pJumpAnimation.Update(elapsedSec);

		if (UpdateJumping())
		{
			m_MyState = TrollState::Falling;
		}
		break;

	case TrollState::Falling:
		m_pJumpAnimation.Update(elapsedSec);

		if (UpdateFalling())
		{
			m_MyState = TrollState::Shooting;
			m_HasFiredThisShot = false;
		}
		break;
	}

	ApplyGravity(elapsedSec);
	ApplyHorisontalMovement(elapsedSec);
}


void Troll::Draw() const
{
	switch (m_MyState)
	{
	case TrollState::Spawning:
		m_pSpawnAnimation.Draw(m_Collider, false, false);
		break;
	case TrollState::Shooting:
		m_pShootAnimation.Draw(m_Collider, m_IsFacingRight);
		break;
	case TrollState::Walking:
		m_pWalkAnimation.Draw(m_Collider, m_IsFacingRight);
		break;
	case TrollState::Jumping:
	case TrollState::Falling:
		m_pJumpAnimation.Draw(m_Collider, m_IsFacingRight);
		break;
	}
}


bool Troll::UpdateWalking(float elapsedSec, const Vector2f& playerPos)
{
	m_WalkTimer += elapsedSec;

	if (m_Collider.left < playerPos.x)
	{
		m_Velocity.x = m_Speed * elapsedSec;
		m_IsFacingRight = true;
	}
	else
	{
		m_Velocity.x = -m_Speed * elapsedSec;
		m_IsFacingRight = false;
	}

	if (m_WalkTimer >= 1.f)
	{
		m_WalkTimer = 0.f;
		m_Velocity.x = 0.f;
		return true;
	}

	return false;
}
void Troll::StartJump()
{
	m_MyState = TrollState::Jumping;
	m_Velocity.y = m_JumpSpeed;
}
bool Troll::UpdateJumping()
{
	if (m_Velocity.y <= 0.f)
	{
		return true;
	}

	return false;
}
bool Troll::UpdateFalling()
{
	if (m_Velocity.y == 0.f)
	{
		return true;
	}

	return false;
}
bool Troll::SpawnUpdate()
{
	return m_pSpawnAnimation.IsFinished();
}
bool Troll::UpdateShooting(float elapsedSec, const Vector2f& playerPos)
{
	if (!m_HasFiredThisShot)
	{
		Fire(playerPos);

		m_HasFiredThisShot = true;
	}

	if (m_pShootAnimation.IsFinished())
	{
		m_HasFiredThisShot = false;
		return true;
	}

	return false;
}

void Troll::ApplyHorisontalMovement(float elapsedSec)
{
	if (m_pVertices == nullptr)
	{
		return;
	}

	utils::HitInfo hitInfo{};
	bool hitWallOnX = false;

	if (m_Velocity.x > 0)
	{
		hitWallOnX = utils::CheckSideCollision(*m_pVertices, m_Collider, utils::Side::right, m_Velocity.x, &hitInfo);

		if (!hitWallOnX)
		{
			m_Collider.left += m_Velocity.x;
		}
		else
		{
			m_Collider.left = hitInfo.intersectPoint.x - m_Collider.width;
		}
	}
	else if (m_Velocity.x < 0)
	{
		hitWallOnX = utils::CheckSideCollision(*m_pVertices, m_Collider, utils::Side::left, -m_Velocity.x, &hitInfo);

		if (!hitWallOnX)
		{
			m_Collider.left += m_Velocity.x;
		}
		else
		{
			m_Collider.left = hitInfo.intersectPoint.x;
		}
	}
}

void Troll::ApplyGravity(float elapsedSec)
{
	if (m_pVertices == nullptr)
	{
		return;
	}

	m_Velocity.y += m_Gravity * elapsedSec;

	if (m_Velocity.y <= 0.f)
	{
		float sweepDist{ -(m_Velocity.y * elapsedSec) };
		utils::HitInfo hitInfo{};
		bool hitGround = utils::CheckSideCollision(*m_pVertices, m_Collider, utils::Side::bottom, sweepDist, &hitInfo);

		if (!hitGround)
		{
			m_Collider.bottom += m_Velocity.y * elapsedSec;
		}
		else
		{
			m_Collider.bottom = hitInfo.intersectPoint.y;
			m_Velocity.y = 0.f;
		}
	}
	else
	{
		m_Collider.bottom += m_Velocity.y * elapsedSec;
	}
}

void Troll::Fire(const Vector2f& playerPos)
{
	if (m_pEntityManager == nullptr)
	{
		return;
	}

	m_IsFacingRight = playerPos.x > m_Collider.left;

	float shootY = m_Collider.bottom + 18.f;

	Vector2f spawnPos
	{
		m_IsFacingRight
			? m_Collider.left + m_Collider.width
			: m_Collider.left,
		shootY
	};

	Vector2f direction
	{
		m_IsFacingRight ? 1.f : -1.f,
		0.f
	};

	m_pEntityManager->SpawnDemonProjectile(spawnPos, direction);
}

void Troll::SetWorld(const std::vector<std::vector<Vector2f>>* vertices)
{
	m_pVertices = vertices;
}
void Troll::SetEntityManager(EntityManager* manager)
{
	m_pEntityManager = manager;
}

