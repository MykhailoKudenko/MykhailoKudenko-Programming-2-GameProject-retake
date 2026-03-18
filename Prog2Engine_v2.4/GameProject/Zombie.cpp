#include "pch.h"
#include "Zombie.h"
#include "utils.h"

Zombie::Zombie(Rectf Start, bool FacingRight) : Enemy(Start)
{
    m_Speed = 30;
    if (!FacingRight)
    {
        m_Speed = m_Speed * -1;

    }
}

void Zombie::Update(float elapsedSec){

	m_Velocity.x = m_Speed * elapsedSec;
	m_Velocity.y = m_Gravity * elapsedSec;

	//collisions right/left
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
			// optional snap against wall
			m_Speed = m_Speed * -1;
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
		// moving left: cast rays from left side outward

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
			// optional snap against wall
			m_Speed = m_Speed * -1;
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
	//collisions gravity

	utils::HitInfo myInfoLeft{};
	utils::HitInfo myInfoRight{};

	bool hitLeft = utils::LoopOverVertecies(*m_pVertices, 
		Vector2f{ m_Collider.left, m_Collider.bottom + m_Collider.height }, 
		Vector2f{ m_Collider.left, m_Collider.bottom - 1.f }, 
		myInfoLeft);
	bool hitRight = utils::LoopOverVertecies(*m_pVertices, 
		Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom + m_Collider.height }, 
		Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom - 1.f }, 
		myInfoRight);

	if (!(hitLeft || hitRight)) 
	{
		m_Collider.bottom += m_Velocity.y;
	}
	else //if not in the air
	{
		bool validLeftHit = hitLeft && myInfoLeft.intersectPoint.y <= m_Collider.bottom - 1.f + 2.f;
		bool validRightHit = hitRight && myInfoRight.intersectPoint.y <= m_Collider.bottom - 1.f + 2.f;

		if (!(validLeftHit || validRightHit)) //if hit is valid
		{
			m_Collider.bottom += m_Velocity.y;


		}
		else //slap to the ground
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
    utils::SetColor(Color4f{ 0, 1, 0, 1 });
    utils::DrawRect(m_Collider);
}



void Zombie::SetWorld(const std::vector<std::vector<Vector2f>>* vertices)
{
    m_pVertices = vertices;
}
