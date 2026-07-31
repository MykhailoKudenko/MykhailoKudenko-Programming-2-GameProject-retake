#include "pch.h"
#include "Zombie.h"
#include "utils.h"

Animation* Zombie::m_pWalkAnimation{ nullptr };
Animation* Zombie::m_pSpawnAnimation{ nullptr };
int Zombie::m_InstanceCount{ 0 };

Zombie::Zombie(Vector2f startPos, bool facingRight)
	: Enemy(Rectf{ startPos.x, startPos.y, 19, 27 })
{
	m_IsFacingRight = facingRight;
	m_Speed = 30.f;
	if (!facingRight)
	{
		m_Speed *= -1.f;
	}

	++m_InstanceCount;

	if (m_pWalkAnimation == nullptr)
	{
		m_pWalkAnimation = new Animation("ZombieWalk.png", 2, 0.13f, true);
	}

	if (m_pSpawnAnimation == nullptr)
	{
		m_pSpawnAnimation = new Animation("ZombieSpawn.png", 3, 0.39f, false);
	}
}

Zombie::~Zombie()
{
	--m_InstanceCount;

	if (m_InstanceCount <= 0)
	{
		delete m_pWalkAnimation;
		m_pWalkAnimation = nullptr;

		delete m_pSpawnAnimation;
		m_pSpawnAnimation = nullptr;

		m_InstanceCount = 0;
	}
}

void Zombie::Update(float elapsedSec)
{
	if (m_pVertices == nullptr)
	{
		return;
	}

	m_AnimTime += elapsedSec;

	if (m_State == ZombieState::Spawning)
	{
		if (m_pSpawnAnimation != nullptr && m_pSpawnAnimation->IsTimeFinished(m_AnimTime))
		{
			m_State = ZombieState::Walking;
			m_AnimTime = 0.f;
		}
		return;
	}


	m_Velocity.x = m_Speed * elapsedSec;
	m_Velocity.y = m_Gravity * elapsedSec;

	// collisions right/left
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
			m_Speed *= -1;
			m_IsFacingRight = (m_Speed > 0);

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
			m_Speed *= -1;
			m_IsFacingRight = (m_Speed > 0);

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
	// collisions gravity
	utils::HitInfo myInfoLeft{};
	utils::HitInfo myInfoRight{};

	bool hitLeft = utils::LoopOverVertecies(
		*m_pVertices,
		Vector2f{ m_Collider.left, m_Collider.bottom + m_Collider.height },
		Vector2f{ m_Collider.left, m_Collider.bottom - 1.f },
		myInfoLeft);

	bool hitRight = utils::LoopOverVertecies(
		*m_pVertices,
		Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom + m_Collider.height },
		Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom - 1.f },
		myInfoRight);

	if (!(hitLeft || hitRight))
	{
		m_Collider.bottom += m_Velocity.y;
	}
	else
	{
		bool validLeftHit = hitLeft && myInfoLeft.intersectPoint.y <= m_Collider.bottom - 1.f + 2.f;
		bool validRightHit = hitRight && myInfoRight.intersectPoint.y <= m_Collider.bottom - 1.f + 2.f;

		if (!(validLeftHit || validRightHit))
		{
			m_Collider.bottom += m_Velocity.y;
		}
		else
		{
			m_Velocity.y = 0;

			if (validLeftHit && validRightHit)
			{
				m_Collider.bottom = std::max(myInfoLeft.intersectPoint.y, myInfoRight.intersectPoint.y);
			}
			else if (validLeftHit)
			{
				m_Collider.bottom = myInfoLeft.intersectPoint.y;
			}
			else
			{
				m_Collider.bottom = myInfoRight.intersectPoint.y;
			}
		}
	}
}



void Zombie::Draw() const
{
	if (m_State == ZombieState::Spawning)
	{
		if (m_pSpawnAnimation != nullptr)
		{
			m_pSpawnAnimation->DrawAtTime(Rectf{ m_Collider.left, m_Collider.bottom, m_pSpawnAnimation->GetFrameWidth(), m_pSpawnAnimation->GetFrameHeight() }, m_AnimTime, m_IsFacingRight);
		}
	}
	else if (m_State == ZombieState::Walking)
	{
		if (m_pWalkAnimation != nullptr)
		{
			m_pWalkAnimation->DrawAtTime(Rectf{ m_Collider.left, m_Collider.bottom, m_pWalkAnimation->GetFrameWidth(), m_pWalkAnimation->GetFrameHeight()}, m_AnimTime, m_IsFacingRight);
		}
	}

}
void Zombie::SetWorld(const std::vector<std::vector<Vector2f>>* vertices)
{
	m_pVertices = vertices;
}


bool Zombie::IsSpawning() const
{
	if (m_State == ZombieState::Spawning)
	{
		return true;
	}
	else
	{
		return false;
	}
}