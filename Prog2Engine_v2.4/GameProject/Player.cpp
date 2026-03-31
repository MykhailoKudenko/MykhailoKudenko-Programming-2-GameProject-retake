#include "pch.h"
#include "Player.h"


//temp
#include <iostream>

Player::Player(Vector2f Pos) : m_Collider{ Rectf{Pos.x, Pos.y, 16, 24} }, 
m_WalkingArmour{ "WalkKnight.png", 3,  0.13f, true }, m_WalkingNaked{ "WalkKnightNaked.png", 3, 0.13f, true }, 
m_ClimbingArmour{ "ClimbKnight.png", 2,  0.13f, true }, m_ClimbingNaked{ "ClimbKnightNaked.png", 2,  0.13f, true },
m_ThrowArmour{"ThrowKnight.png", 2,  0.13f, false }, m_ThrowNaked{"ThrowKnightNaked.png", 2,  0.13f, false },
m_DuckThrowArmour{ "DuckThrowKnight.png", 2,  0.13f, false }, m_DuckThrowNaked{ "DuckThrowKnightNaked.png", 2,  0.13f, false },
m_DuckArmour{ "DuckKnight.png" }, m_DuckNaked{ "DuckKnightNaked.png" }, 
m_JumpingArmour{ "JumpKnight.png"}, m_JumpingNaked{ "JumpKnightNaked.png" },
m_Hit{"HitKnight.png"}
{
}
Player::~Player()
{

}

void Player::Draw() const
{
	
	switch (m_Mystate)
	{
	case PlayerState::Standing:
		if (m_IsWearingArmour)
		m_WalkingArmour.DrawFrame(m_Collider, 0, !m_isFacingRight);
		else
		m_WalkingNaked.DrawFrame(m_Collider, 0, !m_isFacingRight);
		break;

	case PlayerState::Walking:
		if (m_IsWearingArmour)
		m_WalkingArmour.Draw(m_Collider, !m_isFacingRight);
		else
		m_WalkingNaked.Draw(m_Collider, !m_isFacingRight);
		break;
	case PlayerState::ClimbingStill:
		if (m_IsWearingArmour)
			m_ClimbingArmour.DrawFrame(m_Collider, 0, false);
		else
			m_ClimbingNaked.DrawFrame(m_Collider, 0, false);
		break;
	case PlayerState::Climbing:
		if (m_IsWearingArmour)
			m_ClimbingArmour.Draw(m_Collider, false);
		else
			m_ClimbingNaked.Draw(m_Collider, false);
		break;
	case PlayerState::Ducking:
		if (m_IsWearingArmour)
			m_DuckArmour.Draw(Vector2f{ m_isFacingRight ? m_Collider.left : m_Collider.left - m_DuckArmour.GetWidth() + m_Collider.width , m_Collider.bottom }, !m_isFacingRight);
		else
			m_DuckNaked.Draw(Vector2f{ m_isFacingRight ? m_Collider.left : m_Collider.left - m_DuckNaked.GetWidth() + m_Collider.width , m_Collider.bottom }, !m_isFacingRight);
		break;
	case PlayerState::Jumping:
		if (m_IsWearingArmour)
		m_JumpingArmour.Draw(Vector2f{ m_isFacingRight ? m_Collider.left : m_Collider.left - m_JumpingArmour.GetWidth() + m_Collider.width , m_Collider.bottom}, !m_isFacingRight);
		else
		m_JumpingNaked.Draw(Vector2f{ m_isFacingRight ? m_Collider.left : m_Collider.left - m_JumpingNaked.GetWidth() + m_Collider.width , m_Collider.bottom }, !m_isFacingRight);
		break;
	case PlayerState::Knockback:
		m_Hit.Draw(Vector2f{ m_isFacingRight ? m_Collider.left : m_Collider.left - m_Hit.GetWidth() + m_Collider.width , m_Collider.bottom }, !m_isFacingRight);
		break;
	case PlayerState::Throwing:
		if (m_IsWearingArmour)
			m_ThrowArmour.Draw(m_Collider, !m_isFacingRight);
		else
			m_ThrowNaked.Draw(m_Collider, !m_isFacingRight);
		break;
	case PlayerState::DuckingThrow:
		if (m_IsWearingArmour)
			m_DuckThrowArmour.Draw(m_Collider, !m_isFacingRight);
		else
			m_DuckThrowNaked.Draw(m_Collider, !m_isFacingRight);
		break;
	}
	utils::SetColor(Color4f{ 0, 1, 0, 1 });
	utils::DrawRect(m_Collider);
}
void Player::Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices, const std::vector<Rectf>& ladders, std::vector<std::vector<Vector2f>> platfroms)
{
	/*switch (m_Mystate)
	{
	case PlayerState::Standing:
		std::cout << "Standing" << std::endl;
		break;
	case PlayerState::Walking:
		std::cout << "Walking" << std::endl;
		break;
	case PlayerState::Jumping:
		std::cout << "Jumping" << std::endl;
		break;
	case PlayerState::Climbing:
		std::cout << "Climbing" << std::endl;
		break;
	case PlayerState::Knockback:
		std::cout << "Knockback" << std::endl;
		break;
	case PlayerState::Ducking:
		std::cout << "Ducking" << std::endl;
		break;
	case PlayerState::Throwing:
		std::cout << "Throwing" << std::endl;
		break;
	case PlayerState::Dead:
		std::cout << "Dead" << std::endl;
		break;
	default:
		std::cout << "Unknown" << std::endl;
		break;
	}*/
	

	
	//input
	UpdateInput();

	//states
	UpdateStates(ladders ,elapsedSec);

	//timers
	UpdateTimers(elapsedSec);

	//movemnt control + states reset
	UpdateMovmentVertical(vertices, platfroms, elapsedSec);
	UpdateMovmentHorisontal(vertices, elapsedSec);
	
}


Vector2f Player::GetCenterPosition() const
{
	return(Vector2f{ m_Collider.left + m_Collider.width / 2, m_Collider.bottom + m_Collider.height/2 });
}




void Player::UpdateInput()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	m_inputDirectionX = 0;
	m_inputDirectionY = 0;
	m_IsShootButtonPressed = false;

	if (pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D])
	{
		m_inputDirectionX = 1;
	}
	if (pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A])
	{
		m_inputDirectionX = -1;
	}
	
	if (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W] || pStates[SDL_SCANCODE_SPACE])
	{
		m_inputDirectionY = 1;
	}
	if (pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S])
	{
		m_inputDirectionY = -1;
	}

	bool isPressedNow = pStates[SDL_SCANCODE_E];

	m_DoesWantToThrow = (isPressedNow && !m_PreviousShootPressed);

	m_PreviousShootPressed = isPressedNow;

}
void Player::UpdateStates(const std::vector<Rectf>& ladders, float elapsedSec)
{
	// Update facing direction
	if (m_inputDirectionX == 1)
	{
		m_isFacingRight = true;
	}
	else if (m_inputDirectionX == -1)
	{
		m_isFacingRight = false;
	}

	// While the up/down key is still being held after climbing started,
	// ignore new vertical actions until released
	if (m_BlockVerticalActionsUntilReleased
		&& m_Mystate != PlayerState::Climbing
		&& m_Mystate != PlayerState::ClimbingStill)
	{
		if (m_inputDirectionY == 0)
		{
			m_BlockVerticalActionsUntilReleased = false;
		}
		else
		{
			return;
		}
	}

	// Handle throw states first, because they should lock the player
	if (m_Mystate == PlayerState::Throwing)
	{
		Animation& anim = m_IsWearingArmour ? m_ThrowArmour : m_ThrowNaked;
		anim.Update(elapsedSec);

		if (anim.IsFinished())
		{
			m_Mystate = PlayerState::Standing;
		}
		return;
	}
	else if (m_Mystate == PlayerState::DuckingThrow)
	{
		Animation& anim = m_IsWearingArmour ? m_DuckThrowArmour : m_DuckThrowNaked;
		anim.Update(elapsedSec);

		if (anim.IsFinished())
		{
			m_Mystate = PlayerState::Ducking;
		}
		return;
	}

	// Keep climbing state synced with vertical input
	if (m_Mystate == PlayerState::Climbing || m_Mystate == PlayerState::ClimbingStill)
	{
		if (m_inputDirectionY == 0)
		{
			m_Mystate = PlayerState::ClimbingStill;
		}
		else
		{
			m_Mystate = PlayerState::Climbing;
		}
	}

	// Up input: climb if possible, otherwise jump if on ground
	if (m_inputDirectionY == 1)
	{
		if (m_Mystate != PlayerState::Climbing
			&& m_Mystate != PlayerState::ClimbingStill
			&& TryClimb(ladders, true))
		{
			m_Mystate = PlayerState::Climbing;
			SnapToCurrentLadderCenter();
			m_BlockVerticalActionsUntilReleased = true;
			return;
		}
		else if (m_isOnTheGround)
		{
			m_Mystate = PlayerState::Jumping;
			m_JumpTimeUpCurrent = m_JumpTimeUpMax;
			m_JumpDirectionX = m_isFacingRight ? 1 : -1;
			return;
		}
	}

	// Down input: climb down or duck
	if ((m_Mystate == PlayerState::Standing || m_Mystate == PlayerState::Walking)
		&& m_inputDirectionY == -1)
	{
		if (m_Mystate != PlayerState::Climbing
			&& m_Mystate != PlayerState::ClimbingStill
			&& TryClimb(ladders, false))
		{
			m_Mystate = PlayerState::Climbing;
			SnapToCurrentLadderCenter();
			m_BlockVerticalActionsUntilReleased = true;
			return;
		}
		else
		{
			m_Mystate = PlayerState::Ducking;
			return;
		}
	}

	// Stop ducking when down is released
	if (m_Mystate == PlayerState::Ducking && m_inputDirectionY != -1)
	{
		m_Mystate = PlayerState::Standing;
	}

	// Start throwing
	if (m_DoesWantToThrow)
	{
		if (m_Mystate == PlayerState::Ducking)
		{
			m_Mystate = PlayerState::DuckingThrow;

			if (m_IsWearingArmour)
				m_DuckThrowArmour.Reset();
			else
				m_DuckThrowNaked.Reset();
		}
		else if (m_Mystate != PlayerState::Climbing && m_Mystate != PlayerState::ClimbingStill)
		{
			m_Mystate = PlayerState::Throwing;

			if (m_IsWearingArmour)
				m_ThrowArmour.Reset();
			else
				m_ThrowNaked.Reset();
		}

		return;
	}

	// Update looping movement animations
	switch (m_Mystate)
	{
	case PlayerState::Walking:
		if (m_IsWearingArmour)
			m_WalkingArmour.Update(elapsedSec);
		else
			m_WalkingNaked.Update(elapsedSec);
		break;

	case PlayerState::Climbing:
		if (m_IsWearingArmour)
			m_ClimbingArmour.Update(elapsedSec);
		else
			m_ClimbingNaked.Update(elapsedSec);
		break;

	default:
		break;
	}
}

void Player::UpdateTimers(float elapsedSec)
{
	if (m_JumpTimeUpCurrent >= 0)
	{
		m_JumpTimeUpCurrent -= elapsedSec;
	}
	if (m_InvulnerableTimeCurrent >= 0)
	{
		m_InvulnerableTimeCurrent -= elapsedSec;
	}
	if (m_KnockbackTimeCurrent >= 0)
	{
		m_KnockbackTimeCurrent -= elapsedSec;
	}
}

void Player::UpdateMovmentHorisontal(const std::vector<std::vector<Vector2f>>& vertices, float elapsedSec)
{
	float XSpeed{0};
	
	if (m_Mystate == PlayerState::Standing || m_Mystate == PlayerState::Walking)
	{
		XSpeed = m_inputDirectionX * m_MovementSpeed * elapsedSec;
	}
	if (m_Mystate == PlayerState::Knockback)
	{
		XSpeed = m_KnockBackDirectionX * m_KnockBackSpeed * elapsedSec;
	}
	if (m_Mystate == PlayerState::Jumping)
	{
		XSpeed = m_JumpDirectionX * m_JumpSpeed * elapsedSec;
	}
	if (XSpeed == 0
		&& m_Mystate != PlayerState::Ducking
		&& m_Mystate != PlayerState::Climbing
		&& m_Mystate != PlayerState::ClimbingStill
		&& m_Mystate != PlayerState::Throwing
		&& m_Mystate != PlayerState::DuckingThrow
		&& m_Mystate != PlayerState::Jumping
		&& m_Mystate != PlayerState::Knockback)
	{
		m_Mystate = PlayerState::Standing;
		return;
	}
	//collisions right/left
	utils::HitInfo myInfoTopSide{};
	utils::HitInfo myInfoBottomSide{};

	bool hitWallOnX = false;

	if (XSpeed > 0)
	{
		bool hitTopSide = utils::LoopOverVertecies(vertices,Vector2f{ m_Collider.left, m_Collider.bottom + m_Collider.height },Vector2f{ m_Collider.left + m_Collider.width + XSpeed, m_Collider.bottom + m_Collider.height },myInfoTopSide);
		bool hitBottomSide = utils::LoopOverVertecies(vertices,Vector2f{ m_Collider.left, m_Collider.bottom + 1 },Vector2f{ m_Collider.left + m_Collider.width + XSpeed, m_Collider.bottom + 1 },myInfoBottomSide);
		hitWallOnX = hitTopSide || hitBottomSide;
	}
	else if (XSpeed < 0)
	{
		// moving left: cast rays from left side outward

		bool hitTopSide = utils::LoopOverVertecies(vertices,Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom + m_Collider.height },Vector2f{ m_Collider.left + XSpeed, m_Collider.bottom + m_Collider.height },myInfoTopSide);

		bool hitBottomSide = utils::LoopOverVertecies(vertices,Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom + 1 },Vector2f{ m_Collider.left + XSpeed, m_Collider.bottom + 1 },myInfoBottomSide);
		hitWallOnX = hitTopSide || hitBottomSide;
		
	}
	if (!hitWallOnX)
	{
		m_Collider.left += XSpeed;

		if (XSpeed != 0 && (m_Mystate == PlayerState::Standing || m_Mystate == PlayerState::Walking))
		{
			m_Mystate = PlayerState::Walking;
		}
	}
}

void Player::UpdateMovmentVertical(const std::vector<std::vector<Vector2f>>& vertices, std::vector<std::vector<Vector2f>> platfroms, float elapsedSec)
{
	float YSpeed{ 0 };

	if (m_Mystate == PlayerState::Climbing || m_Mystate == PlayerState::ClimbingStill)
	{
		m_Collider.bottom += m_ClimbSpeed * elapsedSec * m_inputDirectionY;
	}
	else if (m_KnockbackTimeCurrent > 0)
	{
		YSpeed = m_KnockBackSpeed * elapsedSec;
	}
	else if (m_JumpTimeUpCurrent > 0)
	{
		YSpeed = m_JumpSpeed * elapsedSec;
	}
	else
	{
		YSpeed = m_Gravity * elapsedSec;
	}

	utils::HitInfo myInfoLeft{};
	utils::HitInfo myInfoRight{};

	bool hitLeft = utils::LoopOverVertecies(vertices, Vector2f{ m_Collider.left, m_Collider.bottom + m_Collider.height }, Vector2f{ m_Collider.left, m_Collider.bottom - YSpeed }, myInfoLeft);
	bool hitRight = utils::LoopOverVertecies(vertices, Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom + m_Collider.height }, Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom - 1.f }, myInfoRight);

	if (!(hitLeft || hitRight)) //if didnt hit check for platforms
	{
		hitLeft = utils::LoopOverVertecies(platfroms, Vector2f{ m_Collider.left, m_Collider.bottom + m_Collider.height }, Vector2f{ m_Collider.left, m_Collider.bottom - YSpeed }, myInfoLeft);
		hitRight = utils::LoopOverVertecies(platfroms, Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom + m_Collider.height }, Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom - 1.f }, myInfoRight);

	}
	if (!(hitLeft || hitRight) || YSpeed > 0  || m_Mystate == PlayerState::Climbing || m_Mystate == PlayerState::ClimbingStill) // if not on the ground
	{
		m_Collider.bottom += YSpeed;
		m_isOnTheGround = false;
	}
	else //if on the ground
	{
		if (m_Mystate == PlayerState::Jumping)
		{
			m_Mystate = PlayerState::Standing;
		}
		if (m_Mystate == PlayerState::Knockback)
		{
			m_Mystate = PlayerState::Standing;
		}
		m_Collider.bottom = std::max(myInfoLeft.intersectPoint.y, myInfoRight.intersectPoint.y);

		m_isOnTheGround = true;
	}

	if (m_Mystate == PlayerState::Climbing && !IsStillOnLadder())
	{
		m_Mystate = PlayerState::Standing;
	}
}

//getters/ setters
Rectf Player::GetHitbox() const
{
	return(m_Collider);
}

void Player::TakeDamage()
{
	if (m_InvulnerableTimeCurrent > 0)
		return;

	m_Mystate = PlayerState::Knockback;
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
	if (m_isFacingRight)
	{
		m_KnockBackDirectionX = -1;
	}
	else
	{
		m_KnockBackDirectionX = 1;
	}

}


bool Player::DoesWantToThrow() const
{
	return m_DoesWantToThrow;
}

bool Player::IsFacingRight() const
{
	return(m_isFacingRight);
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
			ladderHitbox = Rectf{ ladder.left, ladder.bottom + ladder.height / 2, ladder.width, ladder.height / 2 };
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


	if (m_inputDirectionY > 0)
	{
		if (playerBottom > ladderTop)
		{
			return false;
		}
	}

	if (m_inputDirectionY < 0)
	{
		if (playerBottom < ladderBottom)
		{
			return false;
		}
	}

	return true;
}

void Player::SnapToCurrentLadderCenter()
{
	if (m_pCurrentLadder == nullptr)
	{
		return;
	}

	m_Collider.left = m_pCurrentLadder->left
		+ (m_pCurrentLadder->width - m_Collider.width) / 2.0f;
}