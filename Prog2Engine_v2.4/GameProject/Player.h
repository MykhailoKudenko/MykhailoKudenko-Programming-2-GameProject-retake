#pragma once
#include <vector>
class Player
{
public:
	Player(Rectf Start);
	~Player();

	void Draw() const;
	void Update(float elapsedSec, const std::vector<Vector2f>& vertices);

private:
	Rectf m_Collider;
	Vector2f m_Velocity;

	

	const float m_WalkingSpeed = 20;
	const float m_Gravity = -20;
};

