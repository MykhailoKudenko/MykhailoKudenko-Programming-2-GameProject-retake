#include "pch.h"
#include "Ghost.h"
#include "utils.h"
#include "EntityManager.h"

Animation* Ghost::m_pFlyAnimation{ nullptr };
Animation* Ghost::m_pSpawnAnimation{ nullptr };
int Ghost::m_InstanceCount{ 0 };

Ghost::Ghost(Vector2f startPos, bool facingRight)
	: Enemy(Rectf{ startPos.x, startPos.y, 24, 13.f })
	, m_State{ GhostState::Spawning }
{
	m_IsFacingRight = facingRight;
	m_Speed = 40.f;

	if (!facingRight)
	{
		m_Speed *= -1.f;
	}

	++m_InstanceCount;

	if (m_pFlyAnimation == nullptr)
	{
		m_pFlyAnimation = new Animation("GhostFly.png", 2, 0.13f, true);
	}

	if (m_pSpawnAnimation == nullptr)
	{
		m_pSpawnAnimation = new Animation("GhostSpawn.png", 2, 0.39f, false);
	}
	m_EffectType = Effect::EffectType::Fire;

}
Ghost::~Ghost()
{
	--m_InstanceCount;

	if (m_InstanceCount <= 0)
	{
		delete m_pFlyAnimation;
		m_pFlyAnimation = nullptr;

		delete m_pSpawnAnimation;
		m_pSpawnAnimation = nullptr;

		m_InstanceCount = 0;
	}
}

void Ghost::Update(float elapsedSec)
{
	m_AnimTime += elapsedSec;

	if (m_State == GhostState::Spawning)
	{
		if (m_pSpawnAnimation != nullptr && m_pSpawnAnimation->IsTimeFinished(m_AnimTime))
		{
			m_State = GhostState::Flying;
			m_AnimTime = 0.f;
			m_HasPassedPlayerX = false;
		}
		return;
	}

	if (m_State == GhostState::Flying)
	{
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
		if (m_pSpawnAnimation != nullptr)
		{
			m_pSpawnAnimation->DrawAtTime(
				Rectf
				{
					m_Collider.left,
					m_Collider.bottom,
					m_pSpawnAnimation->GetFrameWidth(),
					m_pSpawnAnimation->GetFrameHeight()
				},
				m_AnimTime,
				m_IsFacingRight
			);
		}
	}
	else if (m_State == GhostState::Flying || m_State == GhostState::Dropping)
	{
		if (m_pFlyAnimation != nullptr)
		{
			m_pFlyAnimation->DrawAtTime(
				Rectf
				{
					m_Collider.left,
					m_Collider.bottom,
					m_pFlyAnimation->GetFrameWidth(),
					m_pFlyAnimation->GetFrameHeight()
				},
				m_AnimTime,
				m_IsFacingRight
			);
		}
	}
}

void Ghost::SetEntityManager(EntityManager* manager)
{
	m_pEntityManager = manager;
}


bool Ghost::isSpawning()
{
	if (m_State == GhostState::Spawning)
	{
		return true;
	}
	else
	{
		return false;
	}
}