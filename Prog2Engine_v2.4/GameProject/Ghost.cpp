#include "pch.h"
#include "Ghost.h"
#include "utils.h"
#include "EntityManager.h"


Ghost::Ghost(Vector2f startPos, bool facingRight)
	: Enemy(Rectf{ startPos.x, startPos.y, 24, 13.f })
	, m_State{ GhostState::Spawning }
	, m_pFlyAnimation{ Animation("GhostFly.png", 2, 0.13f, true) }
	, m_pSpawnAnimation{ Animation("GhostSpawn.png", 2, 0.39f, false) }
{
	m_IsFacingRight = facingRight;
	m_Speed = 40.f;

	if (!facingRight)
	{
		m_Speed *= -1.f;
	}

}


void Ghost::Update(float elapsedSec)
{

	if (m_State == GhostState::Spawning)
	{
		m_pSpawnAnimation.Update(elapsedSec);
		if (m_pSpawnAnimation.IsFinished())
		{
			m_State = GhostState::Flying;
			m_HasPassedPlayerX = false;
		}
		return;
	}

	if (m_State == GhostState::Flying)
	{
		m_pFlyAnimation.Update(elapsedSec);
		m_Collider.left += m_Speed * elapsedSec;

		float ghostCenterX = m_Collider.left + m_Collider.width / 2.f;
		float ghostCenterY = m_Collider.bottom + m_Collider.height / 2.f;

		float playerCenterX = m_pEntityManager->GetPlayerPosition().x;
		float playerCenterY = m_pEntityManager->GetPlayerPosition().y;

		const float yTolerance = 4.f;

		if (!m_HasPassedPlayerX)
		{
			if (m_IsFacingRight && ghostCenterX >= playerCenterX)
			{
				m_HasPassedPlayerX = true;
			}
			else if (!m_IsFacingRight && ghostCenterX <= playerCenterX)
			{
				m_HasPassedPlayerX = true;
			}
		}

		if (m_HasPassedPlayerX)
		{
			bool wentFarEnough = false;

			if (m_IsFacingRight && ghostCenterX >= playerCenterX + m_ExtraPastPlayer)
			{
				wentFarEnough = true;
			}
			else if (!m_IsFacingRight && ghostCenterX <= playerCenterX - m_ExtraPastPlayer)
			{
				wentFarEnough = true;
			}

			if (wentFarEnough)
			{
				if (ghostCenterY > playerCenterY + yTolerance)
				{
					m_State = GhostState::Dropping;
					m_TargetBottomAfterDrop = m_Collider.bottom - m_DropDistance;
				}
				else
				{
					m_Speed *= -1.f;
					m_IsFacingRight = (m_Speed > 0.f);
					m_HasPassedPlayerX = false;
				}
			}
		}

		return;
	}

	if (m_State == GhostState::Dropping)
	{
		m_pFlyAnimation.Update(elapsedSec);

		m_Collider.bottom -= m_DropSpeed * elapsedSec;

		if (m_Collider.bottom <= m_TargetBottomAfterDrop)
		{
			m_Collider.bottom = m_TargetBottomAfterDrop;

			m_Speed *= -1.f;
			m_IsFacingRight = (m_Speed > 0.f);
			m_HasPassedPlayerX = false;
			m_State = GhostState::Flying;
		}
	}
}
void Ghost::Draw() const
{
	if (m_State == GhostState::Spawning)
	{
		m_pSpawnAnimation.Draw(m_Collider, false, false);
	}
	else if (m_State == GhostState::Flying || m_State == GhostState::Dropping)
	{
		m_pFlyAnimation.Draw(m_Collider, m_IsFacingRight);
		
	}
}

void Ghost::SetEntityManager(EntityManager* manager)
{
	m_pEntityManager = manager;
}


bool Ghost::IsSpawning() const
{
	return m_State == GhostState::Spawning;
}