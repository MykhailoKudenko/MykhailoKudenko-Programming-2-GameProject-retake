#include "pch.h"
#include "Troll.h"
#include "utils.h"
#include "EntityManager.h"
#include <cmath>

#include <iostream>

Animation* Troll::m_pJumpAnimation{ nullptr };
Animation* Troll::m_pShootAnimation{ nullptr };
Animation* Troll::m_pWalkAnimation{ nullptr };
Animation* Troll::m_pSpawnAnimation{ nullptr };
int Troll::m_InstanceCount{ 0 };


Troll::Troll(Vector2f startPos)
	: Enemy(Rectf{ startPos.x, startPos.y, 43, 40 })
{
	m_Speed = 30.f;
	m_Health = 10;
	m_IsBoss = true;

	++m_InstanceCount;

	if (m_pJumpAnimation == nullptr)
	{
		m_pJumpAnimation = new Animation("TrollJump.png", 1, 0.13f, true);
	}

	if (m_pShootAnimation == nullptr)
	{
		m_pShootAnimation = new Animation("TrollShoot.png", 1, 0.52f, false);
	}
	if (m_pWalkAnimation == nullptr)
	{
		m_pWalkAnimation = new Animation("TrollWalking.png", 2, 0.13f, true);
	}

	if (m_pSpawnAnimation == nullptr)
	{
		m_pSpawnAnimation = new Animation("TrollStand.png", 1, 0.52f, false);
	}

	m_EffectType = Effect::EffectType::Fire;


}
Troll::~Troll()
{
	--m_InstanceCount;

	if (m_InstanceCount <= 0)
	{
		delete m_pJumpAnimation;
		m_pJumpAnimation = nullptr;

		delete m_pShootAnimation;
		m_pShootAnimation = nullptr;

		delete m_pWalkAnimation;
		m_pWalkAnimation = nullptr;

		delete m_pSpawnAnimation;
		m_pSpawnAnimation = nullptr;

		m_InstanceCount = 0;
	}
}

void Troll::Update(float elapsedSec)
{
	m_AnimTime += elapsedSec;

	m_Velocity.x = 0.f;

	switch (m_MyState)
	{
	case TrollState::Spawning:
		if (SpawnUpdate(elapsedSec))
		{
			m_MyState = TrollState::Shooting;
			m_AnimTime = 0.f;
			m_HasFiredThisShot = false;
		}
		break;

	case TrollState::Shooting:
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
				m_AnimTime = 0.f;
				m_WalkTimer = 0.f;
				m_DoJumpNext = true;
			}
		}
		break;

	case TrollState::Walking:
		if (UpdateWalking(elapsedSec, m_pEntityManager->GetPlayerPosition()))
		{
			m_MyState = TrollState::Shooting;
			m_AnimTime = 0.f;
			m_HasFiredThisShot = false;
		}
		break;

	case TrollState::Jumping:
		if (UpdateJumping())
		{
			m_MyState = TrollState::Falling;
			m_AnimTime = 0.f;
		}
		break;

	case TrollState::Falling:
		if (UpdateFalling())
		{
			m_MyState = TrollState::Shooting;
			m_AnimTime = 0.f;
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
				true
			);
		}
		break;
	case TrollState::Shooting:
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
				m_IsFacingRight
			);
		}
		break;
	case TrollState::Walking:
		if (m_pWalkAnimation != nullptr)
		{
			m_pWalkAnimation->DrawAtTime(
				Rectf{
					m_Collider.left,
					m_Collider.bottom,
					m_pWalkAnimation->GetFrameWidth(),
					m_pWalkAnimation->GetFrameHeight()
				},
				m_AnimTime,
				m_IsFacingRight
			);
		}
		break;
	case TrollState::Jumping:
	case TrollState::Falling:
		if (m_pJumpAnimation != nullptr)
		{
			m_pJumpAnimation->DrawAtTime(
				Rectf{
					m_Collider.left,
					m_Collider.bottom,
					m_pJumpAnimation->GetFrameWidth(),
					m_pJumpAnimation->GetFrameHeight()
				},
				m_AnimTime,
				m_IsFacingRight
			);
		}
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
	m_AnimTime = 0.f;
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
bool Troll::SpawnUpdate(float elapsedSec)
{
	if (m_pSpawnAnimation != nullptr && m_pSpawnAnimation->IsTimeFinished(m_AnimTime))
	{
		m_AnimTime = 0.f;
		return true;
	}
	return false;
}
bool Troll::UpdateShooting(float elapsedSec, const Vector2f& playerPos)
{
	if (!m_HasFiredThisShot && m_AnimTime >= 0.2f)
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

void Troll::ApplyHorisontalMovement(float elapsedSec)
{
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

