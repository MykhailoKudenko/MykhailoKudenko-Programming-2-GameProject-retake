#pragma once
#include <vector>
#include "utils.h"

#include "Animation.h"

enum class PlayerState
{
	Standing,
	Walking,
	Jumping,
	Climbing,
	ClimbingStill,
	Knockback,
	Ducking,
	Throwing,
	DuckingThrow,
	Dead
};


class Player
{
public:
	Player(Vector2f Pos);
	~Player();

	void Draw() const;
	void Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices, 
		const std::vector<Rectf>& ladders, 
		std::vector<std::vector<Vector2f>> platfroms); //maybe make by reference later

	Vector2f GetCenterPosition() const;
	Rectf GetHitbox() const;
	void TakeDamage();

	bool DoesWantToThrow() const;
	bool IsFacingRight() const;

private:
	void UpdateStates(const std::vector<Rectf>& ladders, float elapsedSec);
	void UpdateInput();
	void UpdateTimers(float elapsedSec);
	void UpdateMovmentHorisontal(const std::vector<std::vector<Vector2f>>& vertices, float elapsedSec);
	void UpdateMovmentVertical(const std::vector<std::vector<Vector2f>>& vertices, std::vector<std::vector<Vector2f>> platfroms, float elapsedSec);

	bool TryClimb(const std::vector<Rectf>& ladders, bool isGoingUp);
	bool IsStillOnLadder() const;
	void SnapToCurrentLadderCenter();
	//movement(speeds)
	float m_Gravity{-60};
	float m_MovementSpeed{ 30 };
	float m_JumpSpeed{ 60 };
	float m_KnockBackSpeed{ 80 };
	float m_ClimbSpeed{ 40.f };

	//input
	int m_inputDirectionX{ 0 };
	int m_inputDirectionY{ 0 };
	bool m_IsShootButtonPressed{false};
	//POSITION
	bool m_isFacingRight = true;
	bool m_isOnTheGround = false;
	Rectf m_Collider;

	//jumping
	float m_JumpDirectionX{0};
	const float m_JumpTimeUpMax{ 0.3 };
	float m_JumpTimeUpCurrent{ 0 };
	//KonkBack
	const float m_InvulnerableTimeMax{ 0.6 };
	float m_InvulnerableTimeCurrent{ 0 };
	const float	m_KnockbackTimeMax{ 0.2 };
	float m_KnockbackTimeCurrent{ 0 };
	float m_KnockBackDirectionX{ -1 };
	//climbing 
	const Rectf* m_pCurrentLadder = nullptr;
	bool m_BlockVerticalActionsUntilReleased{ false };
	//Animations
	Animation m_WalkingArmour;
	Animation m_ClimbingArmour;
	Animation m_ThrowArmour;
	Animation m_DuckThrowArmour;

	Texture m_JumpingArmour;
	Texture m_DuckArmour;

	Animation m_WalkingNaked;
	Animation m_ClimbingNaked;
	Animation m_ThrowNaked;
	Animation m_DuckThrowNaked;

	Texture m_JumpingNaked;
	Texture m_DuckNaked;

	Texture m_Hit;
	//State machine
	bool m_IsWearingArmour = true;
	PlayerState m_Mystate{ PlayerState::Standing };

	//Throwing
	bool m_PreviousShootPressed{ false };
	bool m_DoesWantToThrow{ false };
	//entity manager
};

