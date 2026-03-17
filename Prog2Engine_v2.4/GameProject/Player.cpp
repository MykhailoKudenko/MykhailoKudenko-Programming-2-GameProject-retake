#include "pch.h"
#include "Player.h"
#include "utils.h"

//temp
#include <iostream>

Player::Player(Rectf Start) : m_Collider{ Start }
{

}
Player::~Player()
{

}

void Player::Draw() const
{
	utils::SetColor(Color4f{ 0, 1, 0, 1 });
	utils::DrawRect(m_Collider);

	

}
void Player::Update(float elapsedSec, const std::vector<Vector2f>& vertices)
{
	m_Velocity.x = 0;

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D])
	{
		m_Velocity.x = m_WalkingSpeed * elapsedSec;
	}
	if (pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A])
	{
		m_Velocity.x = -m_WalkingSpeed * elapsedSec;
	}

	m_Velocity.y = m_Gravity * elapsedSec;

	//collisions right/left
	utils::HitInfo myInfoTopSide{};
	utils::HitInfo myInfoBottomSide{};

	bool hitWallOnX = false;

	if (m_Velocity.x > 0)
	{
		bool hitTopSide = utils::Raycast(
			vertices,
			Vector2f{ m_Collider.left, m_Collider.bottom + m_Collider.height },
			Vector2f{ m_Collider.left + m_Collider.width + m_Velocity.x, m_Collider.bottom + m_Collider.height },
			myInfoTopSide);

		bool hitBottomSide = utils::Raycast(
			vertices,
			Vector2f{ m_Collider.left, m_Collider.bottom },
			Vector2f{ m_Collider.left + m_Collider.width + m_Velocity.x, m_Collider.bottom },
			myInfoBottomSide);
		hitWallOnX = hitTopSide || hitBottomSide;

		if (!hitWallOnX)
		{
			m_Collider.left += m_Velocity.x;
		}
		else
		{
			// optional snap against wall
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

		bool hitTopSide = utils::Raycast(
			vertices,
			Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom + m_Collider.height },
			Vector2f{ m_Collider.left + m_Velocity.x, m_Collider.bottom + m_Collider.height },
			myInfoTopSide);

		bool hitBottomSide = utils::Raycast(
			vertices,
			Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom },
			Vector2f{ m_Collider.left + m_Velocity.x, m_Collider.bottom },
			myInfoBottomSide);

		hitWallOnX = hitTopSide || hitBottomSide;

		if (!hitWallOnX)
		{
			m_Collider.left += m_Velocity.x;
		}
		else
		{
			// optional snap against wall
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
	utils::HitInfo myInfoLeft;
	utils::HitInfo myInfoRight;

	bool hitLeft = utils::Raycast(
		vertices,
		Vector2f{ m_Collider.left, m_Collider.bottom + m_Collider.height },
		Vector2f{ m_Collider.left, m_Collider.bottom - 1.f },
		myInfoLeft);

	bool hitRight = utils::Raycast(
		vertices,
		Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom + m_Collider.height },
		Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom - 1.f },
		myInfoRight);

	

	if (!(hitLeft || hitRight))
	{
		m_Collider.bottom += m_Velocity.y;
	}
	else
	{
		bool validLeftHit = hitLeft && myInfoLeft.intersectPoint.y <= m_Collider.bottom + 1.f;
		bool validRightHit = hitRight && myInfoRight.intersectPoint.y <= m_Collider.bottom + 1.f;

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