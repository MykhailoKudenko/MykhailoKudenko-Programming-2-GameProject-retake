#include "pch.h"
#include "Zombie.h"
#include "utils.h"



Zombie::Zombie(Vector2f startPos, bool facingRight)
	: Enemy(Rectf{ startPos.x, startPos.y, 19, 27 }),
	m_pWalkAnimation{ Animation("ZombieWalk.png", 2, 0.13f, true ) }, 
	m_pSpawnAnimation{ Animation( "ZombieSpawn.png", 3, 0.39f, false ) }
{
	m_IsFacingRight = facingRight;
	m_Speed = 30.f;
	if (!facingRight)
	{
		m_Speed *= -1.f;
	}
	
}


void Zombie::Update(float elapsedSec)
{
	if (m_pVertices == nullptr)
	{
		return;
	}

	switch (m_State)
	{
	case ZombieState::Spawning:
		m_pSpawnAnimation.Update(elapsedSec);
		if (m_pSpawnAnimation.IsFinished())
		{
			m_State = ZombieState::Walking;
		}
		break;
	case ZombieState::Walking:
		m_pWalkAnimation.Update(elapsedSec);

		m_Velocity.x = m_Speed * elapsedSec;
		m_Velocity.y = m_Gravity * elapsedSec;

		utils::HitInfo myInfoTopSide{};
		utils::HitInfo myInfoBottomSide{};

		bool hitWallOnX = false;

		if (m_Velocity.x > 0)
		{

			hitWallOnX = utils::CheckSideCollision(*m_pVertices, m_Collider, utils::Side::right, m_Velocity.x);



			if (!hitWallOnX)
			{
				m_Collider.left += m_Velocity.x;
			}
			else
			{
				m_Speed *= -1;
				m_IsFacingRight = (m_Speed > 0);
			}
		}
		else if (m_Velocity.x < 0)
		{
			bool hitWallOnX = utils::CheckSideCollision(*m_pVertices, m_Collider, utils::Side::left, -m_Velocity.x);


			if (!hitWallOnX)
			{
				m_Collider.left += m_Velocity.x;
			}
			else
			{
				m_Speed *= -1;
				m_IsFacingRight = (m_Speed > 0);

			}
		}
		utils::HitInfo gravityHit{};
		if (!utils::CheckSideCollision(*m_pVertices, m_Collider, utils::Side::bottom, 2.f, &gravityHit))
		{
			m_Collider.bottom += m_Velocity.y;
		}
		else
		{
			m_Velocity.y = 0;
			m_Collider.bottom = gravityHit.intersectPoint.y;
		}
		break;
	}


	
}



void Zombie::Draw() const
{
	switch (m_State)
	{
	case ZombieState::Spawning:
		m_pSpawnAnimation.Draw(m_Collider, m_IsFacingRight, false);
		break;
	case ZombieState::Walking:
		m_pWalkAnimation.Draw(m_Collider, m_IsFacingRight);
		break;
	}
}

void Zombie::SetWorld(const std::vector<std::vector<Vector2f>>* vertices)
{
	m_pVertices = vertices;
}


bool Zombie::IsSpawning() const
{
	return m_State == ZombieState::Spawning;
}