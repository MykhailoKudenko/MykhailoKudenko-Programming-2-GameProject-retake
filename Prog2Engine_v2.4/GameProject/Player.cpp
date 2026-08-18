#include "pch.h"
#include "Player.h"

#include "TextureManager.h"
#include "EntityManager.h"

Player::Player(const Vector2f& startPos, EntityManager* manager) :
	m_pEntityManager{ manager },
	m_MovementSpeed{ 50 },
	m_JumpSpeed{ 60 },
	m_KnockbackSpeed{ 80 },
	m_ClimbSpeed{ 60.f },
	m_InputDirectionX{ 0 },
	m_InputDirectionY{ 0 },
	m_IsShootButtonPressed{ false },
	m_IsFacingRight{ true },
	m_IsOnTheGround{ false },
	m_Collider{ startPos.x, startPos.y, 16, 24 },
	m_JumpDirectionX{ 0.f },
	m_JumpTimeUpMax{ 0.3f },
	m_JumpTimeUpCurrent{ 0.f },
	m_InvulnerableTimeMax{ 1.f },
	m_InvulnerableTimeCurrent{ 0.f },
	m_KnockbackTimeMax{ 0.2f },
	m_KnockbackTimeCurrent{ 0.f },
	m_KnockbackDirectionX{ -1.f },
	m_pCurrentLadder{ nullptr },
	m_BlockVerticalActionsUntilReleased{ false },
	m_WalkingArmour{ "WalkKnight.png", 3, 0.10f, true },
	m_ClimbingArmour{ "ClimbKnight.png", 2, 0.10f, true },
	m_ThrowArmour{ "ThrowKnight.png", 2, 0.10f, false },
	m_DuckThrowArmour{ "DuckThrowKnight.png", 2, 0.10f, false },
	m_pJumpingArmour{ TextureManager::GetInstance().GetTexture("JumpKnight.png") },
	m_pDuckArmour{ TextureManager::GetInstance().GetTexture("DuckKnight.png") },
	m_WalkingNaked{ "WalkKnightNaked.png", 3, 0.10f, true },
	m_ClimbingNaked{ "ClimbKnightNaked.png", 2, 0.10f, true },
	m_ThrowNaked{ "ThrowKnightNaked.png", 2, 0.10f, false },
	m_DuckThrowNaked{ "DuckThrowKnightNaked.png", 2, 0.10f, false },
	m_pJumpingNaked{ TextureManager::GetInstance().GetTexture("JumpKnightNaked.png") },
	m_pDuckNaked{ TextureManager::GetInstance().GetTexture("DuckKnightNaked.png") },
	m_pHit{ TextureManager::GetInstance().GetTexture("HitKnight.png") },
	m_pDeathKnockback{ TextureManager::GetInstance().GetTexture("DeadKnockBack.png") },
	m_pDeath{ TextureManager::GetInstance().GetTexture("DeadGround.png") },
	m_IsWearingArmour{ true },
	m_MyState{ PlayerState::Standing },
	m_PreviousShootPressed{ false },
	m_DoesWantToThrow{ false },
	m_ThrowCooldownMax{ 0.2f },
	m_ThrowCooldownCurrent{ 0.0f },
	m_MyWeapon{ PlayerWeapon::Lance },
	m_Score{ 0 },
	m_IsImmortal{ false },
	m_DeathTimerMax{ 1.f },
	m_DeathTimeCurrent{ 1.f },
	m_IsFlying{ false },
	m_FlySpeed{ 120.f }
{
}


void Player::Draw() const
{

	switch (m_MyState)
	{
	case PlayerState::Standing:
	case PlayerState::Walking:
		(m_IsWearingArmour ? m_WalkingArmour : m_WalkingNaked).Draw(m_Collider, !m_IsFacingRight);
		break;
	case PlayerState::ClimbingStill:
	case PlayerState::Climbing:
		(m_IsWearingArmour ? m_ClimbingArmour : m_ClimbingNaked).Draw(m_Collider, !m_IsFacingRight);
		break;
	case PlayerState::Ducking:
		(m_IsWearingArmour ? m_pDuckArmour : m_pDuckNaked)->Draw(m_Collider, !m_IsFacingRight, false);
		break;
	case PlayerState::Jumping:
		(m_IsWearingArmour ? m_pJumpingArmour : m_pJumpingNaked)->Draw(m_Collider, !m_IsFacingRight, false);
		break;
	case PlayerState::Knockback:
		m_pHit->Draw(m_Collider, !m_IsFacingRight, false);
		break;
	case PlayerState::Throwing:
		(m_IsWearingArmour ? m_ThrowArmour : m_ThrowNaked).Draw(m_Collider, !m_IsFacingRight);
		break;
	case PlayerState::DuckingThrow:
		(m_IsWearingArmour ? m_DuckThrowArmour : m_DuckThrowNaked).Draw(m_Collider, !m_IsFacingRight, false);
		break;
	case PlayerState::Dead:
		m_pDeath->Draw(m_Collider, !m_IsFacingRight, false);
		break;
	case PlayerState::KnockbackDead:
		m_pDeathKnockback->Draw(m_Collider, !m_IsFacingRight, false);
		break;
	}
}

void Player::Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices, const std::vector<std::vector<Vector2f>>& playerOnlyVertices, const std::vector<Rectf>& ladders, const std::vector<std::vector<Vector2f>>& platforms)
{
	//input

	UpdateInput();

	//states
	UpdateStates(ladders, elapsedSec);


	//timers
	UpdateTimers(elapsedSec);

	//movemnt control + states reset
	UpdateMovementVertical(vertices, playerOnlyVertices, platforms, elapsedSec);
	UpdateMovementHorizontal(vertices, playerOnlyVertices, elapsedSec);
}


Vector2f Player::GetCenterPosition() const
{
	return Vector2f{ m_Collider.left + m_Collider.width / 2, m_Collider.bottom + m_Collider.height / 2 };
}
void Player::UpdateInput()
{
	if (m_MyState == PlayerState::Dead || m_MyState == PlayerState::KnockbackDead)
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
		m_MyState == PlayerState::Climbing ||
		m_MyState == PlayerState::ClimbingStill;

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
			m_MyState = PlayerState::Throwing;

			if (m_IsWearingArmour)
			{
				m_ThrowArmour.Reset();
			}
			else
			{
				m_ThrowNaked.Reset();
			}
			Throw();
			return;
		}

		if (m_InputDirectionX != 0 || m_InputDirectionY != 0)
		{
			m_MyState = PlayerState::Walking;
		}
		else
		{
			m_MyState = PlayerState::Standing;
		}

		return;
	}

	// While the up/down key is still being held after climbing started,
	// ignore new vertical actions until released
	if (m_BlockVerticalActionsUntilReleased
		&& m_MyState != PlayerState::Climbing
		&& m_MyState != PlayerState::ClimbingStill)
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
	if (m_MyState == PlayerState::Throwing)
	{
		Animation& anim = m_IsWearingArmour ? m_ThrowArmour : m_ThrowNaked;
		anim.Update(elapsedSec);

		if (anim.IsFinished())
		{
			m_MyState = PlayerState::Standing;
		}
		return;
	}
	else if (m_MyState == PlayerState::DuckingThrow)
	{
		Animation& anim = m_IsWearingArmour ? m_DuckThrowArmour : m_DuckThrowNaked;
		anim.Update(elapsedSec);

		if (anim.IsFinished())
		{
			m_MyState = PlayerState::Ducking;
		}
		return;
	}

	// Keep climbing state synced with vertical input
	if (m_MyState == PlayerState::Climbing || m_MyState == PlayerState::ClimbingStill)
	{
		if (m_InputDirectionY == 0)
		{
			m_MyState = PlayerState::ClimbingStill;
		}
		else
		{
			m_MyState = PlayerState::Climbing;
		}
	}

	// Up input: climb if possible, otherwise jump if on ground
	if (m_InputDirectionY == 1)
	{
		if (m_MyState != PlayerState::Climbing
			&& m_MyState != PlayerState::ClimbingStill
			&& TryClimb(ladders, true))
		{
			m_MyState = PlayerState::Climbing;
			SnapToCurrentLadderCenter();
			m_BlockVerticalActionsUntilReleased = true;
			return;
		}
		else if (m_IsOnTheGround)
		{
			m_MyState = PlayerState::Jumping;
			m_JumpTimeUpCurrent = m_JumpTimeUpMax;
			m_JumpDirectionX = m_IsFacingRight ? 1.f : -1.f;
			return;
		}
	}

	// Down input: climb down or duck
	if ((m_MyState == PlayerState::Standing || m_MyState == PlayerState::Walking)
		&& m_InputDirectionY == -1)
	{
		if (m_MyState != PlayerState::Climbing
			&& m_MyState != PlayerState::ClimbingStill
			&& TryClimb(ladders, false))
		{
			m_MyState = PlayerState::Climbing;
			SnapToCurrentLadderCenter();
			m_BlockVerticalActionsUntilReleased = true;
			return;
		}
		else
		{
			m_MyState = PlayerState::Ducking;
			return;
		}
	}

	// Stop ducking when down is released
	if (m_MyState == PlayerState::Ducking && m_InputDirectionY != -1)
	{
		m_MyState = PlayerState::Standing;
	}

	// Start throwing
	if (m_DoesWantToThrow)
	{
		if (m_MyState == PlayerState::Ducking)
		{
			m_MyState = PlayerState::DuckingThrow;

			if (m_IsWearingArmour)
			{
				m_DuckThrowArmour.Reset();

			}
			else
			{
				m_DuckThrowNaked.Reset();
			}
			Throw();

		}
		else if (m_MyState != PlayerState::Climbing && m_MyState != PlayerState::ClimbingStill)
		{
			m_MyState = PlayerState::Throwing;

			if (m_IsWearingArmour)
			{
				m_ThrowArmour.Reset();
			}
			else
			{
				m_ThrowNaked.Reset();
			}
			Throw();
		}

		return;
	}

	// Update looping movement animations
	switch (m_MyState)
	{
	case PlayerState::Walking:
		m_IsWearingArmour ? m_WalkingArmour.Update(elapsedSec) : m_WalkingNaked.Update(elapsedSec);
		break;
	case PlayerState::Standing:
		m_IsWearingArmour ? m_WalkingArmour.Reset() : m_WalkingNaked.Reset();
		break;
	case PlayerState::Climbing:
		m_IsWearingArmour ? m_ClimbingArmour.Update(elapsedSec) : m_ClimbingNaked.Update(elapsedSec);
		break;
	case PlayerState::ClimbingStill:
		m_IsWearingArmour ? m_ClimbingArmour.Reset() : m_ClimbingNaked.Reset();
		break;
	default:
		break;
	}
}
void Player::Throw()
{
	if (m_pEntityManager == nullptr)
	{
		return;
	}
	m_pEntityManager->SpawnPlayerWeapon(GetThrowPosition(), m_IsFacingRight, m_MyWeapon);
}
namespace 
{ 
	void Countdown(float& timer, float elapsedSec) 
	{ 
		if (timer >= 0.f) 
		{ 
			timer -= elapsedSec; 
		} 
	} 
}

void Player::UpdateTimers(float elapsedSec)
{
	Countdown(m_JumpTimeUpCurrent, elapsedSec);
	Countdown(m_InvulnerableTimeCurrent, elapsedSec);
	Countdown(m_KnockbackTimeCurrent, elapsedSec);
	Countdown(m_ThrowCooldownCurrent, elapsedSec);

	if (m_MyState == PlayerState::Dead)
	{
		Countdown(m_DeathTimeCurrent, elapsedSec);
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

	if (m_MyState == PlayerState::Standing || m_MyState == PlayerState::Walking)
	{
		xSpeed = m_InputDirectionX * m_MovementSpeed * elapsedSec;
	}
	else if (m_MyState == PlayerState::Knockback || m_MyState == PlayerState::KnockbackDead)
	{
		xSpeed = m_KnockbackDirectionX * m_KnockbackSpeed * elapsedSec;
	}
	else if (m_MyState == PlayerState::Jumping)
	{
		xSpeed = m_JumpDirectionX * m_JumpSpeed * elapsedSec;
	}

	if (xSpeed == 0.f && (m_MyState == PlayerState::Standing || m_MyState == PlayerState::Walking))
	{
		m_MyState = PlayerState::Standing;
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
			&& (m_MyState == PlayerState::Standing || m_MyState == PlayerState::Walking))
		{
			m_MyState = PlayerState::Walking;
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

	if (m_MyState == PlayerState::Climbing || m_MyState == PlayerState::ClimbingStill)
	{
		m_Collider.bottom += m_ClimbSpeed * elapsedSec * m_InputDirectionY;
	}
	else if (m_KnockbackTimeCurrent > 0.f)
	{
		ySpeed = m_KnockbackSpeed * elapsedSec;
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
	utils::HitInfo bottomHitInfo{};

	if (ySpeed <= 0.f)
	{
		float sweepDist{ -ySpeed };
		if (utils::CheckSideCollision(vertices, insetCollider, utils::Side::bottom, sweepDist, &bottomHitInfo) ||
			utils::CheckSideCollision(playerOnlyVertices, insetCollider, utils::Side::bottom, sweepDist, &bottomHitInfo) ||
			utils::CheckSideCollision(platforms, insetCollider, utils::Side::bottom, sweepDist, &bottomHitInfo))
		{
			hitGround = true;
		}
	}
	if (!hitGround || ySpeed > 0.f || m_MyState == PlayerState::Climbing || m_MyState == PlayerState::ClimbingStill)
	{
		m_Collider.bottom += ySpeed;
		m_IsOnTheGround = false;
	}
	else
	{
		if (m_MyState == PlayerState::Jumping)
		{
			m_MyState = PlayerState::Standing;
		}
		if (m_MyState == PlayerState::Knockback)
		{
			m_MyState = PlayerState::Standing;
		}
		if (m_MyState == PlayerState::KnockbackDead)
		{
			m_MyState = PlayerState::Dead;
		}
		m_Collider.bottom = bottomHitInfo.intersectPoint.y;
		m_IsOnTheGround = true;
	}

	if (m_MyState == PlayerState::Climbing && !IsStillOnLadder())
	{
		m_MyState = PlayerState::Standing;
	}
}
Rectf Player::GetHitbox() const
{
	if (m_MyState == PlayerState::Ducking)
	{
		return Rectf{ m_Collider.left, m_Collider.bottom, m_Collider.width, m_Collider.height - 16 };
	}
	else
	{
		return m_Collider;
	}
}

void Player::TakeDamage()
{
	if (m_MyState == PlayerState::Dead)
	{
		return;
	}
	if (m_InvulnerableTimeCurrent > 0)
	{
		return;
	}

	m_MyState = PlayerState::Knockback;
	if (m_IsWearingArmour)
	{
		m_IsWearingArmour = false;
	}
	else
	{
		if (m_IsImmortal == false)
		{
			m_MyState = PlayerState::KnockbackDead;
			m_DeathTimeCurrent = m_DeathTimerMax;
		}
	}

	m_InvulnerableTimeCurrent = m_InvulnerableTimeMax;
	m_KnockbackTimeCurrent = m_KnockbackTimeMax;
	if (m_IsFacingRight)
	{
		m_KnockbackDirectionX = -1;
	}
	else
	{
		m_KnockbackDirectionX = 1;
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
	return (m_MyState == PlayerState::Dead || m_MyState == PlayerState::KnockbackDead) && m_DeathTimeCurrent <= 0.f;
}

bool Player::IsStillOnLadder() const
{
	if (!m_pCurrentLadder)
	{
		return false;
	}

	const Rectf& ladder = *m_pCurrentLadder;

	const float playerBottom = m_Collider.bottom;
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

void Player::SetPlayerWeapon(Player::PlayerWeapon weapon)
{
	m_MyWeapon = weapon;
}
int Player::GetPlayerScore() const
{
	return m_Score;
}
void Player::AddToPlayerScore(int score)
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
	m_MyState = PlayerState::Standing;
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

	if (m_MyState == PlayerState::Ducking || m_MyState == PlayerState::DuckingThrow)
	{
		yOffset = 8.f;
	}

	return Vector2f{
		m_Collider.left + xOffset,
		m_Collider.bottom + yOffset
	};
}
Player::PlayerWeapon Player::GetPlayerWeapon() const
{
	return m_MyWeapon;
}