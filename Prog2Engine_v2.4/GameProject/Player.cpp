#include "pch.h"
#include "Player.h"


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
	

	if (m_KnockbackTimeCurrent >= 0)
	{
		utils::SetColor(Color4f{ 1, 0, 0, 1 });
	}
	else if (m_InvulnerableTimeCurrent >= 0)
	{
		utils::SetColor(Color4f{ 1, 1, 0, 1 });
	}

	utils::DrawRect(m_Collider);
}
void Player::Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices, const std::vector<Rectf>& ladders)
{
	m_Velocity.x = 0;
	m_Velocity.y = 0;
	if (m_KnockbackTimeCurrent >= 0)
	{
		m_KnockbackTimeCurrent -= elapsedSec;
	}
	if (m_InvulnerableTimeCurrent >= 0)
	{
		m_InvulnerableTimeCurrent -= elapsedSec;
	}
	if (m_JumpingTimeCurrent >= 0)
	{
		m_JumpingTimeCurrent -= elapsedSec;
	}
	// left / right
	if (m_KnockbackTimeCurrent < 0)
	{
		UpdateInput(elapsedSec, ladders);
	}

	
	//Jump/Gravity/kockback logic
	if (m_IsClimbing)
	{
		m_Velocity.x = 0;
		if (IsStillOnLadder() == false)
		{
			m_IsClimbing = false;
			m_pCurrentLadder = nullptr;
			m_BlockJumpUntilUpReleased = true;
		}
	}
	else if (m_KnockbackTimeCurrent >= 0)
	{
		m_Velocity = m_KnockBackDirection * m_KnockBackSpeed * elapsedSec;
	}
	else if (m_JumpingTimeCurrent >= 0)
	{
		m_Velocity.y = m_JumpingSpeed * elapsedSec;
	}
	else
	{
		m_Velocity.y = m_Gravity * elapsedSec;
	}
	
	
	CheckCollisionVertical(vertices);
	CheckCollisionHorisontal(vertices);
	
}


Vector2f Player::GetCenterPosition() const
{
	return(Vector2f{ m_Collider.left + m_Collider.width / 2, m_Collider.bottom + m_Collider.height });
}

Rectf Player::GetHitbox() const
{
	return(m_Collider);
}

void Player::TakeDamage()
{
	if (m_InvulnerableTimeCurrent > 0)
		return;
	if (m_IsWearingArmour)
	{
		m_IsWearingArmour = false;
	}
	else
	{
		//idk like die or smth
	}

	m_InvulnerableTimeCurrent = m_InvulnerableTimeMax;
	m_KnockbackTimeCurrent = m_KnockbackTimeMax;
	if (m_Velocity.x > 0)
	{
		m_KnockBackDirection.x = -1;
	}
	else if (m_Velocity.x < 0)
	{
		m_KnockBackDirection.x = 1;
	}
	else
	{
		m_KnockBackDirection.x = 0;
	}
}

bool Player::DoesWantToThrow() const
{
	return(m_DoesWantToThrow);
}

bool Player::IsFacingRight() const
{
	return(m_isFacingRight);
}


void Player::UpdateInput(float elapsedSec, const std::vector<Rectf>& ladders)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	const bool upPressed =
		pStates[SDL_SCANCODE_UP] ||
		pStates[SDL_SCANCODE_W] ||
		pStates[SDL_SCANCODE_SPACE];

	const bool downPressed =
		pStates[SDL_SCANCODE_DOWN] ||
		pStates[SDL_SCANCODE_S];

	if (!upPressed)
	{
		m_BlockJumpUntilUpReleased = false;
	}

	if (pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D])
	{
		m_Velocity.x = m_WalkingSpeed * elapsedSec;
		m_isFacingRight = true;
	}
	if (pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A])
	{
		m_Velocity.x = -m_WalkingSpeed * elapsedSec;
		m_isFacingRight = false;
	}

	if (upPressed)
	{
		if (!m_IsClimbing)
		{
			if (TryClimb(ladders, true))
			{
				m_IsClimbing = true;
				m_Velocity.y = m_ClimbSpeed * elapsedSec;
			}
			else if (m_IsOnTheGround && !m_BlockJumpUntilUpReleased)
			{
				m_JumpingTimeCurrent = m_JumpingTimeMax;
				m_IsOnTheGround = false;
			}
		}
		else
		{
			m_Velocity.y = m_ClimbSpeed * elapsedSec;
		}
	}

	if (downPressed)
	{
		if (!m_IsClimbing)
		{
			if (TryClimb(ladders, false))
			{
				m_IsClimbing = true;
				m_Velocity.y = -m_ClimbSpeed * elapsedSec;
			}
		}
		else
		{
			m_Velocity.y = -m_ClimbSpeed * elapsedSec;
		}
	}

	bool isPressed = pStates[SDL_SCANCODE_E];
	m_DoesWantToThrow = (isPressed && !m_WasThrowPressedLastFrame);
	m_WasThrowPressedLastFrame = isPressed;
}

void Player::CheckCollisionHorisontal(const std::vector<std::vector<Vector2f>>& vertices)
{
	//collisions right/left
	utils::HitInfo myInfoTopSide{};
	utils::HitInfo myInfoBottomSide{};

	bool hitWallOnX = false;

	if (m_Velocity.x > 0)
	{
		bool hitTopSide = utils::LoopOverVertecies(
			vertices,
			Vector2f{ m_Collider.left, m_Collider.bottom + m_Collider.height },
			Vector2f{ m_Collider.left + m_Collider.width + m_Velocity.x, m_Collider.bottom + m_Collider.height },
			myInfoTopSide);

		bool hitBottomSide = utils::LoopOverVertecies(
			vertices,
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
			vertices,
			Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom + m_Collider.height },
			Vector2f{ m_Collider.left + m_Velocity.x, m_Collider.bottom + m_Collider.height },
			myInfoTopSide);

		bool hitBottomSide = utils::LoopOverVertecies(
			vertices,
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
void Player::CheckCollisionVertical(const std::vector<std::vector<Vector2f>>& vertices)
{
	utils::HitInfo myInfoLeft{};
	utils::HitInfo myInfoRight{};

	bool hitLeft = utils::LoopOverVertecies(vertices, Vector2f{ m_Collider.left, m_Collider.bottom + m_Collider.height }, Vector2f{ m_Collider.left, m_Collider.bottom - 1.f }, myInfoLeft);
	bool hitRight = utils::LoopOverVertecies(vertices, Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom + m_Collider.height }, Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom - 1.f }, myInfoRight);

	if (!(hitLeft || hitRight) || m_JumpingTimeCurrent >= 0 ||  m_KnockbackTimeCurrent >= 0 || m_IsClimbing) //if in the air
	{
		m_Collider.bottom += m_Velocity.y;
		m_IsOnTheGround = false;
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
			m_IsOnTheGround = true;
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

bool Player::TryClimb(const std::vector<Rectf>& ladders, bool isGoingUp)
{
	m_pCurrentLadder = nullptr;

	for (const Rectf& ladder : ladders)
	{
		Rectf ladderHitbox;
		if (isGoingUp)
		{
			ladderHitbox = Rectf{ ladder.left, ladder.bottom, ladder.width, ladder.height / 2 };
		}
		else
		{
			ladderHitbox = Rectf{ ladder.left, ladder.bottom+ ladder.height / 2, ladder.width, ladder.height / 2 };
		}
		
		
		if (utils::IsOverlapping(ladderHitbox, m_Collider))
		{
			m_pCurrentLadder = &ladder;
			return true;
		}
	}
	return false;
}

bool Player::IsStillOnLadder() const
{
	if (!m_pCurrentLadder) return false;

	const Rectf& ladder = *m_pCurrentLadder;

	const float playerBottom = m_Collider.bottom;
	const float playerTop = m_Collider.bottom + m_Collider.height;
	const float ladderBottom = ladder.bottom;
	const float ladderTop = ladder.bottom + ladder.height;


	if (m_Velocity.y > 0)
	{
		if (playerBottom > ladderTop)
		{
			return false;
		}
	}

	if (m_Velocity.y < 0)
	{
		if (playerBottom < ladderBottom)
		{
			return false;
		}
	}

	return true;
}