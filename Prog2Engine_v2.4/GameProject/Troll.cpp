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


Troll::Troll(Vector2f StartPos)
	: Enemy(Rectf{ StartPos.x, StartPos.y, 43, 40 })
{
	m_Speed = 30.f;
	m_health = 20;
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
	utils::HitInfo myInfoTopSide{};
	utils::HitInfo myInfoBottomSide{};

	bool hitWallOnX = false;

	if (m_Velocity.x > 0)
	{
		bool hitTopSide = utils::LoopOverVertecies(
			*m_pVertices,
			Vector2f{ m_Collider.left, m_Collider.bottom + m_Collider.height },
			Vector2f{ m_Collider.left + m_Collider.width + m_Velocity.x, m_Collider.bottom + m_Collider.height },
			myInfoTopSide);

		bool hitBottomSide = utils::LoopOverVertecies(
			*m_pVertices,
			Vector2f{ m_Collider.left, m_Collider.bottom + 1 },
			Vector2f{ m_Collider.left + m_Collider.width + m_Velocity.x, m_Collider.bottom + 1 },
			myInfoBottomSide);

		hitWallOnX = hitTopSide || hitBottomSide;

		if (!hitWallOnX)
		{
			m_Collider.left += m_Velocity.x;
		}
		else
		{
			
			if (hitTopSide && hitBottomSide)
			{
				m_Collider.left = std::min(myInfoTopSide.intersectPoint.x, myInfoBottomSide.intersectPoint.x) - m_Collider.width;
			}
			else if (hitTopSide)
			{
				m_Collider.left = myInfoTopSide.intersectPoint.x - m_Collider.width;
			}
			else
			{
				m_Collider.left = myInfoBottomSide.intersectPoint.x - m_Collider.width;
			}
		}
	}
	else if (m_Velocity.x < 0)
	{
		bool hitTopSide = utils::LoopOverVertecies(
			*m_pVertices,
			Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom + m_Collider.height },
			Vector2f{ m_Collider.left + m_Velocity.x, m_Collider.bottom + m_Collider.height },
			myInfoTopSide);

		bool hitBottomSide = utils::LoopOverVertecies(
			*m_pVertices,
			Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom + 1 },
			Vector2f{ m_Collider.left + m_Velocity.x, m_Collider.bottom + 1 },
			myInfoBottomSide);

		hitWallOnX = hitTopSide || hitBottomSide;

		if (!hitWallOnX)
		{
			m_Collider.left += m_Velocity.x;
		}
		else
		{
			if (hitTopSide && hitBottomSide)
			{
				m_Collider.left = std::max(myInfoTopSide.intersectPoint.x, myInfoBottomSide.intersectPoint.x);
			}
			else if (hitTopSide)
			{
				m_Collider.left = myInfoTopSide.intersectPoint.x;
			}
			else
			{
				m_Collider.left = myInfoBottomSide.intersectPoint.x;
			}
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

	utils::HitInfo hitInfoLeft{};
	utils::HitInfo hitInfoRight{};

	bool hitLeft = utils::LoopOverVertecies(
		*m_pVertices,
		Vector2f{ m_Collider.left, m_Collider.bottom + m_Collider.height },
		Vector2f{ m_Collider.left, m_Collider.bottom + m_Velocity.y * elapsedSec },
		hitInfoLeft);

	bool hitRight = utils::LoopOverVertecies(
		*m_pVertices,
		Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom + m_Collider.height },
		Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom + m_Velocity.y * elapsedSec },
		hitInfoRight);

	if (!(hitLeft || hitRight))
	{
		m_Collider.bottom += m_Velocity.y * elapsedSec;
	}
	else
	{
		if (m_Velocity.y <= 0.f)
		{
			if (hitLeft && hitRight)
			{
				m_Collider.bottom = std::max(hitInfoLeft.intersectPoint.y, hitInfoRight.intersectPoint.y);
			}
			else if (hitLeft)
			{
				m_Collider.bottom = hitInfoLeft.intersectPoint.y;
			}
			else
			{
				m_Collider.bottom = hitInfoRight.intersectPoint.y;
			}

			m_Velocity.y = 0.f;
		}
		else
		{
			m_Collider.bottom += m_Velocity.y * elapsedSec;
		}
	}
}

void Troll::Fire(const Vector2f& playerPos)
{
	if (m_pEntityManager == nullptr)
	{
		return;
	}

	Vector2f spawnPos
	{
		m_Collider.left + m_Collider.width / 2.f,
		m_Collider.bottom + m_Collider.height / 2.f
	};

	Vector2f direction
	{
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


void Troll::SetWorld(const std::vector<std::vector<Vector2f>>* vertices)
{
	m_pVertices = vertices;
}
void Troll::SetEntityManager(EntityManager* manager)
{
	m_pEntityManager = manager;
}

