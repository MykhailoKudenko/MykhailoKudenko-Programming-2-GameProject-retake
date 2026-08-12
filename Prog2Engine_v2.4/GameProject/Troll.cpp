#include "pch.h"
#include "Troll.h"
#include "utils.h"
#include "EntityManager.h"
#include <cmath>

Troll::Troll(Vector2f startPos, const std::vector<std::vector<Vector2f>>* vertices, EntityManager* manager)
	: Enemy(Rectf{ startPos.x, startPos.y, 43, 40 }, 30, false, 300, false, Effect::EffectType::Fire, SoundManager::SFX::None, true, 10),
	m_JumpAnimation{ "TrollJump.png", 1, 0.13f, true },
	m_ShootAnimation{ "TrollShoot.png", 1, 0.52f, false },
	m_WalkAnimation{ "TrollWalking.png", 2, 0.13f, true },
	m_SpawnAnimation{ "TrollStand.png", 1, 0.52f, false },
	m_pEntityManager{ manager },
	m_pVertices{ vertices },
	m_MyState{ TrollState::Spawning },
	m_WalkTimerMax{1.f},
	m_WalkTimerCurrent{ 0.f },
	m_DoJumpNext{ false },
	m_IsGrounded{ false },
	m_JumpTimeMax{0.3f},
	m_JumpTimCurrent{0.f},
	m_JumpSpeed{ 60.f }
{
}


void Troll::Update(float elapsedSec)
{
	switch (m_MyState)
	{
	case TrollState::Spawning:
		m_SpawnAnimation.Update(elapsedSec);

		if (m_SpawnAnimation.IsFinished())
		{
			m_MyState = TrollState::Shooting;
			Fire();

		}
		break;

	case TrollState::Shooting:
		m_ShootAnimation.Update(elapsedSec);

		if (m_ShootAnimation.IsFinished())
		{
			m_ShootAnimation.Reset();
			if (m_DoJumpNext)
			{
				m_MyState = TrollState::Jumping;
				m_DoJumpNext = false;
			}
			else
			{
				m_MyState = TrollState::Walking;
				m_DoJumpNext = true;
			}
		}
		break;

	case TrollState::Walking:
		m_WalkAnimation.Update(elapsedSec);
		m_WalkTimerCurrent += elapsedSec;

		if (!m_IsGrounded)
		{
			m_MyState = TrollState::Falling;
			m_WalkTimerCurrent = 0;
		}
		else if (m_WalkTimerCurrent >= m_WalkTimerMax)
		{
			m_MyState = TrollState::Shooting;
			Fire();
			m_WalkTimerCurrent = 0;

		}
		break;

	case TrollState::Jumping:
		m_JumpAnimation.Update(elapsedSec);
		m_JumpTimCurrent += elapsedSec;
		if (m_JumpTimCurrent >= m_JumpTimeMax)
		{
			m_MyState = TrollState::Falling;
			m_JumpTimCurrent = 0;
		}
		break;

	case TrollState::Falling:
		m_JumpAnimation.Update(elapsedSec);

		if (m_IsGrounded)
		{
			m_MyState = TrollState::Shooting;
			Fire();
		}
		break;
	}

	ApplyVerticalmovement(elapsedSec);
	ApplyHorisontalMovement(elapsedSec);
}


void Troll::Draw() const
{
	switch (m_MyState)
	{
	case TrollState::Spawning:
		m_SpawnAnimation.Draw(m_Collider, false, false);
		break;
	case TrollState::Shooting:
		m_ShootAnimation.Draw(m_Collider, m_IsFacingRight);
		break;
	case TrollState::Walking:
		m_WalkAnimation.Draw(m_Collider, m_IsFacingRight);
		break;
	case TrollState::Jumping:
	case TrollState::Falling:
		m_JumpAnimation.Draw(m_Collider, m_IsFacingRight);
		break;
	}
}

void Troll::ApplyHorisontalMovement(float elapsedSec)
{
	if (m_pVertices == nullptr)
	{
		return;
	}

	utils::HitInfo hitInfo{};
	bool hitWallOnX = false;

	switch (m_MyState)
	{
	case TrollState::Spawning:
	case TrollState::Jumping:
	case TrollState::Falling:
	case TrollState::Shooting:
		m_Velocity.x = 0;
		break;
	case TrollState::Walking:

		if ((m_pEntityManager->GetPlayerPosition().x > GetCenterPosition().x))
		{
			m_Velocity.x = m_Speed * elapsedSec;
		}
		else
		{
			m_Velocity.x = -m_Speed * elapsedSec;
		}
		break;
	}


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

void Troll::ApplyVerticalmovement(float elapsedSec)
{
	if (m_pVertices == nullptr)
	{
		return;
	}

	switch (m_MyState)
	{
	case TrollState::Spawning:
	case TrollState::Shooting:
	case TrollState::Walking:
	case TrollState::Falling:
		m_Velocity.y = utils::g_Gravity;
		break;
	case TrollState::Jumping:
		m_Velocity.y = m_JumpSpeed;
		break;
	}
	if (m_Velocity.y <= 0.f)
	{
		float sweepDist{ m_Velocity.y * elapsedSec };
		utils::HitInfo hitInfo{};
		bool hitGround = utils::CheckSideCollision(*m_pVertices, m_Collider, utils::Side::bottom, std::abs(sweepDist), &hitInfo);

		if (!hitGround)
		{
			m_Collider.bottom += m_Velocity.y * elapsedSec;
		}
		else
		{
			m_IsGrounded = true;
			m_Collider.bottom = hitInfo.intersectPoint.y;
			m_Velocity.y = 0.f;
		}
	}
	else
	{
		m_IsGrounded = false;
		m_Collider.bottom += m_Velocity.y * elapsedSec;
	}
}

void Troll::Fire()
{
	if (m_pEntityManager == nullptr)
	{
		return;
	}
	m_IsFacingRight = m_pEntityManager->GetPlayerPosition().x > m_Collider.left;

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

	m_pEntityManager->SpawnEnemyProjectile(spawnPos, direction.Normalized(), SimpleProjectile::SimpleProjectileType::Troll);
}

