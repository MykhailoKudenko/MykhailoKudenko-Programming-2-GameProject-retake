#pragma once
#include <vector>
#include "utils.h"

class Player
{
public:
	Player(Rectf Start);
	~Player();

	void Draw() const;
	void Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices);

	Vector2f GetCenterPosition() const;

private:

	
	Rectf m_Collider;

	//movement
	Vector2f m_Velocity;
	//left/right	
	const float m_WalkingSpeed = 30;

	//up down
	const float m_JumpingSpeed = 60;
	const float m_JumpingTimeMax = 4;
	float m_JumpingTimeCurrent = 0;
	const float m_Gravity = -60;
	bool m_IsOnTheGround = true;
};

