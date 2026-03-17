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

private:

	bool LoopOverVertecies(const std::vector<std::vector<Vector2f>>& vertices, const Vector2f& rayP1, const Vector2f& rayP2, utils::HitInfo& myInfo);

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

