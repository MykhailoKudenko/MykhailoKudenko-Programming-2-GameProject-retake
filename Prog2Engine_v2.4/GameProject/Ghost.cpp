#include "pch.h"
#include "Ghost.h"
#include "utils.h"
#include "EntityManager.h"


Ghost::Ghost(const Vector2f& startPos, bool facingRight, EntityManager* manager)
	: Enemy(Rectf{ startPos.x, startPos.y, 24, 13.f }, facingRight ? 40.f : -40.f, facingRight, 100, false, Effect::EffectType::Blood, SoundManager::SFX::Ghost),
	m_MyState{ GhostState::Spawning },
	m_DropDistance{ 30.f },
	m_DropSpeed{ 20.f },
	m_ExtraPastPlayer{ 80.f },
	m_HasPassedPlayerX{ false },
	m_TargetBottomAfterDrop{ 0.f },
	m_FlyAnimation{ "GhostFly.png", 2, 0.13f, true },
	m_SpawnAnimation{ "GhostSpawn.png", 2, 0.39f, false },
	m_pEntityManager{ manager }
{	
}


void Ghost::Update(float elapsedSec)
{

	if (m_MyState == GhostState::Spawning)
	{
		m_SpawnAnimation.Update(elapsedSec);
		if (m_SpawnAnimation.IsFinished())
		{
			m_MyState = GhostState::Flying;
			m_HasPassedPlayerX = false;
		}
		return;
	}

	if (m_MyState == GhostState::Flying)
	{
		m_FlyAnimation.Update(elapsedSec);
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
					m_MyState = GhostState::Dropping;
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

	if (m_MyState == GhostState::Dropping)
	{
		m_FlyAnimation.Update(elapsedSec);

		m_Collider.bottom -= m_DropSpeed * elapsedSec;

		if (m_Collider.bottom <= m_TargetBottomAfterDrop)
		{
			m_Collider.bottom = m_TargetBottomAfterDrop;

			m_Speed *= -1.f;
			m_IsFacingRight = (m_Speed > 0.f);
			m_HasPassedPlayerX = false;
			m_MyState = GhostState::Flying;
		}
	}
}
void Ghost::Draw() const
{
	if (m_MyState == GhostState::Spawning)
	{
		m_SpawnAnimation.Draw(m_Collider, false, false);
	}
	else if (m_MyState == GhostState::Flying || m_MyState == GhostState::Dropping)
	{
		m_FlyAnimation.Draw(m_Collider, m_IsFacingRight);
		
	}
}
bool Ghost::IsSpawning() const
{
	return m_MyState == GhostState::Spawning;
}