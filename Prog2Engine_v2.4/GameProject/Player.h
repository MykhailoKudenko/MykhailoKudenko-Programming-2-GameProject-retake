#pragma once
#include <vector>
#include "utils.h"

class Player
{
public:
	Player(Rectf Start);
	~Player();

	void Draw() const;
	void Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices, const std::vector<Rectf>& ladders);

	Vector2f GetCenterPosition() const;
	Rectf GetHitbox() const;
	void TakeDamage();

	bool DoesWantToThrow() const;
	bool IsFacingRight() const;
private:
	void UpdateInput(float elapsedSec, const std::vector<Rectf>& ladders);
	void CheckCollisionHorisontal(const std::vector<std::vector<Vector2f>>& vertices);
	void CheckCollisionVertical(const std::vector<std::vector<Vector2f>>& vertices);
	bool TryClimb(const std::vector<Rectf>& ladders, bool isGoingUp);
	bool IsStillOnLadder() const;
	Rectf m_Collider;

	//movement
	Vector2f m_Velocity;
	//left/right	
	const float m_WalkingSpeed = 90;

	//up down
	const float m_JumpingSpeed = 60;
	const float m_JumpingTimeMax = 0.5;
	float m_JumpingTimeCurrent = 0;
	const float m_Gravity = -60;
	bool m_IsOnTheGround = true;

	//Taking damage
	bool m_IsWearingArmour = true;

	const float m_InvulnerableTimeMax = 1;
	float m_InvulnerableTimeCurrent = 0;

	const float m_KnockbackTimeMax = 0.5;
	float m_KnockbackTimeCurrent = 0;

	const float m_KnockBackSpeed = 50;
	Vector2f m_KnockBackDirection{0, 1};
	//throwing
	bool m_isFacingRight = true;
	bool m_DoesWantToThrow = false;
	bool m_WasThrowPressedLastFrame = false;
	//climbing
	bool m_IsClimbing = false;
	const Rectf* m_pCurrentLadder = nullptr;
	float m_ClimbSpeed{ 80.f };
	bool m_BlockJumpUntilUpReleased = false;

};

