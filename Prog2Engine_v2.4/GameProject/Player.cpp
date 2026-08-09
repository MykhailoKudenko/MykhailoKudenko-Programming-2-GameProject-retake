#include "pch.h"
#include "Player.h"


//temp
#include <iostream>
#include "TextureManager.h"

Player::Player(Vector2f startPos) : m_Collider{ Rectf{startPos.x, startPos.y, 16, 24} },
m_WalkingArmour{ "WalkKnight.png", 3,  0.10f, true },
m_WalkingNaked{ "WalkKnightNaked.png", 3, 0.10f, true },

m_ClimbingArmour{ "ClimbKnight.png", 2,  0.10f, true },
m_ClimbingNaked{ "ClimbKnightNaked.png", 2,  0.10f, true },

m_ThrowArmour{ "ThrowKnight.png", 2,  0.10f, false },
m_ThrowNaked{ "ThrowKnightNaked.png", 2,  0.10f, false },

m_DuckThrowArmour{ "DuckThrowKnight.png", 2,  0.10f, false },
m_DuckThrowNaked{ "DuckThrowKnightNaked.png", 2,  0.10f, false },

m_DuckArmour{ TextureManager::GetInstance().GetTexture("DuckKnight.png") },
m_DuckNaked{ TextureManager::GetInstance().GetTexture("DuckKnightNaked.png") },

m_JumpingArmour{ TextureManager::GetInstance().GetTexture("JumpKnight.png") },
m_JumpingNaked{ TextureManager::GetInstance().GetTexture("JumpKnightNaked.png") },

m_Hit{ TextureManager::GetInstance().GetTexture("HitKnight.png") },
m_DeathKnockBack{ TextureManager::GetInstance().GetTexture("DeadKnockBack.png") },
m_Death{ TextureManager::GetInstance().GetTexture("DeadGround.png") }
{
}


void Player::Draw() const
{

	switch (m_Mystate)
	{
	case PlayerState::Standing:
		if (m_IsWearingArmour)
		{

			m_WalkingArmour.Draw(m_Collider, !m_IsFacingRight);
		}
		else
		{
			m_WalkingNaked.Draw(m_Collider, !m_IsFacingRight);
		}
		break;

	case PlayerState::Walking:
		if (m_IsWearingArmour)
		{
			m_WalkingArmour.Draw(m_Collider, !m_IsFacingRight);
		}
		else
		{
			m_WalkingNaked.Draw(m_Collider, !m_IsFacingRight);
		}
		break;
	case PlayerState::ClimbingStill:
		if (m_IsWearingArmour)
		{
			m_ClimbingArmour.Draw(m_Collider, false);
		}
		else
		{
			m_ClimbingNaked.Draw(m_Collider, false);
		}
		break;
	case PlayerState::Climbing:
		if (m_IsWearingArmour)
		{
			m_ClimbingArmour.Draw(m_Collider, false);
		}
		else
		{
			m_ClimbingNaked.Draw(m_Collider, false);
		}
		break;
	case PlayerState::Ducking:
		if (m_IsWearingArmour)
		{
			m_DuckArmour->Draw(Vector2f{ m_IsFacingRight ? m_Collider.left : m_Collider.left - m_DuckArmour->GetWidth() + m_Collider.width , m_Collider.bottom }, !m_IsFacingRight);
		}
		else
		{
			m_DuckNaked->Draw(Vector2f{ m_IsFacingRight ? m_Collider.left : m_Collider.left - m_DuckNaked->GetWidth() + m_Collider.width , m_Collider.bottom }, !m_IsFacingRight);
		}
		break;
	case PlayerState::Jumping:
		if (m_IsWearingArmour)
		{
			m_JumpingArmour->Draw(Vector2f{ m_IsFacingRight ? m_Collider.left : m_Collider.left - m_JumpingArmour->GetWidth() + m_Collider.width , m_Collider.bottom }, !m_IsFacingRight);
		}
		else
		{
			m_JumpingNaked->Draw(Vector2f{ m_IsFacingRight ? m_Collider.left : m_Collider.left - m_JumpingNaked->GetWidth() + m_Collider.width , m_Collider.bottom }, !m_IsFacingRight);
		}
		break;
	case PlayerState::Knockback:
		m_Hit->Draw(Vector2f{ m_IsFacingRight ? m_Collider.left : m_Collider.left - m_Hit->GetWidth() + m_Collider.width , m_Collider.bottom }, !m_IsFacingRight);
		break;
	case PlayerState::Throwing:
		if (m_IsWearingArmour)
		{
			m_ThrowArmour.Draw(m_Collider, !m_IsFacingRight);
		}
		else
		{
			m_ThrowNaked.Draw(m_Collider, !m_IsFacingRight);
		}
		break;
	case PlayerState::DuckingThrow:
		if (m_IsWearingArmour)
		{
			m_DuckThrowArmour.Draw(Rectf{ m_Collider.left, m_Collider.bottom, m_DuckThrowArmour.GetFrameWidth(), m_DuckThrowArmour.GetFrameHeight() }, !m_IsFacingRight);
		}
		else
		{
			m_DuckThrowNaked.Draw(Rectf{ m_Collider.left, m_Collider.bottom, m_DuckThrowArmour.GetFrameWidth(), m_DuckThrowArmour.GetFrameHeight() }, !m_IsFacingRight);
		}
		break;
	case PlayerState::Dead:
		m_Death->Draw(Vector2f{ m_IsFacingRight ? m_Collider.left : m_Collider.left - m_Death->GetWidth() + m_Collider.width , m_Collider.bottom }, !m_IsFacingRight);
		break;
	case PlayerState::KnockbackDead:
		m_DeathKnockBack->Draw(Vector2f{ m_IsFacingRight ? m_Collider.left : m_Collider.left - m_Hit->GetWidth() + m_Collider.width , m_Collider.bottom }, !m_IsFacingRight);
		break;
	}
}
void Player::Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices, const std::vector<std::vector<Vector2f>>& playerOnlyVertices, const std::vector<Rectf>& ladders, const std::vector<std::vector<Vector2f>>& platfroms)
{
	//input

	UpdateInput();

	//states
	UpdateStates(ladders, elapsedSec);


	//timers
	UpdateTimers(elapsedSec);

	//movemnt control + states reset

	UpdateMovementVertical(vertices, playerOnlyVertices, platfroms, elapsedSec);
	UpdateMovementHorizontal(vertices, playerOnlyVertices, elapsedSec);

}


Vector2f Player::GetCenterPosition() const
{
	return Vector2f{ m_Collider.left + m_Collider.width / 2, m_Collider.bottom + m_Collider.height / 2 };
}




void Player::UpdateInput()
{
	if (m_Mystate == PlayerState::Dead || m_Mystate == PlayerState::KnockbackDead)
	{
		m_InputDirectionX = 0;
		m_InputDirectionY = 0;
		m_IsShootButtonPressed = false;
		m_DoesWantToThrow = false;
		return;
	}

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	m_InputDirectionX = 0;
	m_InputDirectionY = 0;
	m_IsShootButtonPressed = false;

	if (pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D])
	{
		m_InputDirectionX = 1;
	}
	if (pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A])
	{
		m_InputDirectionX = -1;
	}

	if (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W] || pStates[SDL_SCANCODE_SPACE])
	{
		m_InputDirectionY = 1;
	}
	if (pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S])
	{
		m_InputDirectionY = -1;
	}

	bool isPressedNow = pStates[SDL_SCANCODE_E];

	bool isOnLadder =
		m_Mystate == PlayerState::Climbing ||
		m_Mystate == PlayerState::ClimbingStill;

	if (isPressedNow && !m_PreviousShootPressed && m_ThrowCooldownCurrent <= 0 && !isOnLadder)
	{
		m_DoesWantToThrow = true;
		m_ThrowCooldownCurrent = m_ThrowCooldownMax;
	}
	else
	{
		m_DoesWantToThrow = false;
	}
	m_PreviousShootPressed = isPressedNow;

}
void Player::UpdateStates(const std::vector<Rectf>& ladders, float elapsedSec)
{
	// Update facing direction
	if (m_InputDirectionX == 1)
	{
		m_IsFacingRight = true;
	}
	else if (m_InputDirectionX == -1)
	{
		m_IsFacingRight = false;
	}
	//flying debug update
	if (m_IsFlying)
	{
		if (m_DoesWantToThrow)
		{
			m_Mystate = PlayerState::Throwing;

			if (m_IsWearingArmour)
			{
				m_ThrowArmour.Reset();
			}
			else
			{
				m_ThrowNaked.Reset();
			}

			return;
		}

		if (m_InputDirectionX != 0 || m_InputDirectionY != 0)
		{
			m_Mystate = PlayerState::Walking;
		}
		else
		{
			m_Mystate = PlayerState::Standing;
		}

		return;
	}

	// While the up/down key is still being held after climbing started,
	// ignore new vertical actions until released
	if (m_BlockVerticalActionsUntilReleased
		&& m_Mystate != PlayerState::Climbing
		&& m_Mystate != PlayerState::ClimbingStill)
	{
		if (m_InputDirectionY == 0)
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
		if (m_InputDirectionY == 0)
		{
			m_Mystate = PlayerState::ClimbingStill;
		}
		else
		{
			m_Mystate = PlayerState::Climbing;
		}
	}

	// Up input: climb if possible, otherwise jump if on ground
	if (m_InputDirectionY == 1)
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
		else if (m_IsOnTheGround)
		{
			m_Mystate = PlayerState::Jumping;
			m_JumpTimeUpCurrent = m_JumpTimeUpMax;
			m_JumpDirectionX = m_IsFacingRight ? 1 : -1;
			return;
		}
	}

	// Down input: climb down or duck
	if ((m_Mystate == PlayerState::Standing || m_Mystate == PlayerState::Walking)
		&& m_InputDirectionY == -1)
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
	if (m_Mystate == PlayerState::Ducking && m_InputDirectionY != -1)
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
			{
				m_DuckThrowArmour.Reset();

			}
			else
			{
				m_DuckThrowNaked.Reset();
			}
		}
		else if (m_Mystate != PlayerState::Climbing && m_Mystate != PlayerState::ClimbingStill)
		{
			m_Mystate = PlayerState::Throwing;

			if (m_IsWearingArmour)
			{
				m_ThrowArmour.Reset();
			}
			else
			{
				m_ThrowNaked.Reset();
			}
		}

		return;
	}

	// Update looping movement animations
	switch (m_Mystate)
	{
	case PlayerState::Walking:
		if (m_IsWearingArmour)
		{
			m_WalkingArmour.Update(elapsedSec);
		}
		else
		{
			m_WalkingNaked.Update(elapsedSec);
		}
		break;
	case PlayerState::Standing:
		if (m_IsWearingArmour)
		{
			m_WalkingArmour.Reset();
		}
		else
		{
			m_WalkingNaked.Reset();
		}
		break;
	case PlayerState::Climbing:
		if (m_IsWearingArmour)
		{
			m_ClimbingArmour.Update(elapsedSec);
		}
		else
		{
			m_ClimbingNaked.Update(elapsedSec);

		}
		break;
	case PlayerState::ClimbingStill:
		if (m_IsWearingArmour)
		{
			m_ClimbingArmour.Reset();
		}
		else
		{
			m_ClimbingNaked.Reset();
		}
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
	if (m_Mystate == PlayerState::Dead)
	{
		if (m_DeathTimeCurrent >= 0)
			m_DeathTimeCurrent -= elapsedSec;
	}
	if (m_ThrowCooldownCurrent >= 0)
	{
		m_ThrowCooldownCurrent -= elapsedSec;
	}
}

void Player::UpdateMovementHorizontal(
	const std::vector<std::vector<Vector2f>>& vertices,
	const std::vector<std::vector<Vector2f>>& playerOnlyVertices,
	float elapsedSec)
{
	if (m_IsFlying)
	{
		m_Collider.left += m_InputDirectionX * m_FlySpeed * elapsedSec;
		return;
	}

	float xSpeed{ 0.f };

	if (m_Mystate == PlayerState::Standing || m_Mystate == PlayerState::Walking)
	{
		xSpeed = m_InputDirectionX * m_MovementSpeed * elapsedSec;
	}
	else if (m_Mystate == PlayerState::Knockback || m_Mystate == PlayerState::KnockbackDead)
	{
		xSpeed = m_KnockBackDirectionX * m_KnockBackSpeed * elapsedSec;
	}
	else if (m_Mystate == PlayerState::Jumping)
	{
		xSpeed = m_JumpDirectionX * m_JumpSpeed * elapsedSec;
	}

	if (xSpeed == 0.f
		&& m_Mystate != PlayerState::Ducking
		&& m_Mystate != PlayerState::Climbing
		&& m_Mystate != PlayerState::ClimbingStill
		&& m_Mystate != PlayerState::Throwing
		&& m_Mystate != PlayerState::DuckingThrow
		&& m_Mystate != PlayerState::Jumping
		&& m_Mystate != PlayerState::Knockback
		&& m_Mystate != PlayerState::KnockbackDead
		&& m_Mystate != PlayerState::Dead)
	{
		m_Mystate = PlayerState::Standing;
		return;
	}

	bool hitWallOnX{ false };

	if (xSpeed > 0.f)
	{
		Rectf insetCollider{ m_Collider.left + 1, m_Collider.bottom + 1.f, m_Collider.width, m_Collider.height - 1.f };

		bool hitWorld = utils::CheckSideCollision(vertices, insetCollider, utils::Side::right, xSpeed);
		bool hitPlayer = utils::CheckSideCollision(playerOnlyVertices, insetCollider, utils::Side::right, xSpeed);

		hitWallOnX = hitWorld || hitPlayer;
	}
	else if (xSpeed < 0.f)
	{
		Rectf insetCollider{ m_Collider.left - 1, m_Collider.bottom + 1.f, m_Collider.width, m_Collider.height - 1.f };

		bool hitWorld = utils::CheckSideCollision(vertices, insetCollider, utils::Side::left, -xSpeed);
		bool hitPlayer = utils::CheckSideCollision(playerOnlyVertices, insetCollider, utils::Side::left, -xSpeed);

		hitWallOnX = hitWorld || hitPlayer;
	}

	if (!hitWallOnX)
	{
		m_Collider.left += xSpeed;

		if (xSpeed != 0.f
			&& (m_Mystate == PlayerState::Standing || m_Mystate == PlayerState::Walking))
		{
			m_Mystate = PlayerState::Walking;
		}
	}
}

void Player::UpdateMovementVertical(
	const std::vector<std::vector<Vector2f>>& vertices,
	const std::vector<std::vector<Vector2f>>& playerOnlyVertices,
	const std::vector<std::vector<Vector2f>>& platforms,
	float elapsedSec)
{
	if (m_IsFlying)
	{
		m_Collider.bottom += m_InputDirectionY * m_FlySpeed * elapsedSec;
		m_IsOnTheGround = false;
		return;
	}

	float ySpeed{ 0.f };

	if (m_Mystate == PlayerState::Climbing || m_Mystate == PlayerState::ClimbingStill)
	{
		m_Collider.bottom += m_ClimbSpeed * elapsedSec * m_InputDirectionY;
	}
	else if (m_KnockbackTimeCurrent > 0.f)
	{
		ySpeed = m_KnockBackSpeed * elapsedSec;
	}
	else if (m_JumpTimeUpCurrent > 0.f)
	{
		ySpeed = m_JumpSpeed * elapsedSec;
	}
	else
	{
		ySpeed = utils::g_Gravity * elapsedSec;
	}


	Rectf insetCollider{ m_Collider.left + 1.f, m_Collider.bottom, m_Collider.width - 2.f, m_Collider.height };

	bool hitGround = false;
	utils::HitInfo BottomHitInfo{};

	if (ySpeed <= 0.f)
	{
		float sweepDist{ -ySpeed };
		if (utils::CheckSideCollision(vertices, insetCollider, utils::Side::bottom, sweepDist, &BottomHitInfo) ||
			utils::CheckSideCollision(playerOnlyVertices, insetCollider, utils::Side::bottom, sweepDist, &BottomHitInfo) ||
			utils::CheckSideCollision(platforms, insetCollider, utils::Side::bottom, sweepDist, &BottomHitInfo))
		{
			hitGround = true;
		}
	}
	if (!hitGround || ySpeed > 0.f || m_Mystate == PlayerState::Climbing || m_Mystate == PlayerState::ClimbingStill)
	{
		m_Collider.bottom += ySpeed;
		m_IsOnTheGround = false;
	}
	else
	{

		if (m_Mystate == PlayerState::Jumping)
		{
			m_Mystate = PlayerState::Standing;
		}
		if (m_Mystate == PlayerState::Knockback)
		{
			m_Mystate = PlayerState::Standing;
		}
		if (m_Mystate == PlayerState::KnockbackDead)
		{
			m_Mystate = PlayerState::Dead;
		}
		m_Collider.bottom = BottomHitInfo.intersectPoint.y;
		m_IsOnTheGround = true;
	}

	if (m_Mystate == PlayerState::Climbing && !IsStillOnLadder())
	{
		m_Mystate = PlayerState::Standing;
	}
}

//getters/ setters
Rectf Player::GetHitbox() const
{
	if (m_Mystate == PlayerState::Ducking)
	{
		return(Rectf{ m_Collider.left, m_Collider.bottom, m_Collider.width, m_Collider.height - 16 });
	}
	else
	{
		return(m_Collider);
	}
}

void Player::TakeDamage()
{
	if (m_Mystate == PlayerState::Dead)
	{
		return;
	}
	if (m_InvulnerableTimeCurrent > 0)
	{
		return;
	}

	m_Mystate = PlayerState::Knockback;
	if (m_IsWearingArmour)
	{
		m_IsWearingArmour = false;
	}
	else
	{
		if (m_IsImmortal == false)
		{
			m_Mystate = PlayerState::KnockbackDead;
			m_DeathTimeCurrent = m_DeathTimerMax;
		}
	}

	m_InvulnerableTimeCurrent = m_InvulnerableTimeMax;
	m_KnockbackTimeCurrent = m_KnockbackTimeMax;
	if (m_IsFacingRight)
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
	return(m_IsFacingRight);
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


bool Player::IsDeathAnimationFinished() const
{
	return (m_Mystate == PlayerState::Dead || m_Mystate == PlayerState::KnockbackDead)
		&& m_DeathTimeCurrent <= 0.f;
}

bool Player::IsStillOnLadder() const
{
	if (!m_pCurrentLadder)
	{
		return false;
	}

	const Rectf& ladder = *m_pCurrentLadder;

	const float playerBottom = m_Collider.bottom;
	const float playerTop = m_Collider.bottom + m_Collider.height;
	const float ladderBottom = ladder.bottom;
	const float ladderTop = ladder.bottom + ladder.height;


	if (m_InputDirectionY > 0)
	{
		if (playerBottom > ladderTop)
		{
			return false;
		}
	}

	if (m_InputDirectionY < 0)
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

Player::PlayerWeapon Player::GetPlayerWeapon() const
{
	return(m_MyWeapon);
}
void Player::SetPlayerWeapon(Player::PlayerWeapon weapon)
{
	m_MyWeapon = weapon;
}
int Player::GetPlayerScore() const
{
	return m_Score;
}
void Player::AddToPLayerScore(int score)
{
	m_Score += score;
}
void Player::SetPlayerScore(int newScore)
{
	m_Score = newScore;
}

void Player::Respawn(const Vector2f& pos)
{
	m_Collider = Rectf{ pos.x, pos.y, 16.f, 24.f };
	m_Mystate = PlayerState::Standing;
	m_IsWearingArmour = true;

	m_DeathTimeCurrent = m_DeathTimerMax;
	m_InvulnerableTimeCurrent = 0.f;
	m_KnockbackTimeCurrent = 0.f;
	m_JumpTimeUpCurrent = 0.f;

	m_InputDirectionX = 0;
	m_InputDirectionY = 0;
	m_DoesWantToThrow = false;
	m_PreviousShootPressed = false;
	m_pCurrentLadder = nullptr;
}

void Player::SetImmortal(bool isImmortal)
{
	m_IsImmortal = isImmortal;
}
bool Player::IsImmortal() const
{
	return m_IsImmortal;
}

void Player::SetFlying(bool isFlying)
{
	m_IsFlying = isFlying;
}

bool Player::IsFlying() const
{
	return m_IsFlying;
}

Vector2f Player::GetThrowPosition() const
{
	float xOffset = 0;
	if (m_IsFacingRight)
	{
		xOffset = m_Collider.width;
	}
	else
	{
		xOffset = -m_Collider.width;
	}

	float yOffset = 16.f;

	if (m_Mystate == PlayerState::Ducking || m_Mystate == PlayerState::DuckingThrow)
	{
		yOffset = 8.f;
	}

	return Vector2f{
		m_Collider.left + xOffset,
		m_Collider.bottom + yOffset
	};
}
