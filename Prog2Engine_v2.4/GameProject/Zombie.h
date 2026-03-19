#pragma once
#include "Enemy.h"

class Zombie : public Enemy
{
public:
	Zombie(Rectf Start, bool FacingRight);

	void Update(float elapsedSec) override;

	void Draw() const override;

	void SetWorld(const std::vector<std::vector<Vector2f>>* vertices);

private:
	const std::vector<std::vector<Vector2f>>* m_pVertices{}; //mase static later
	static constexpr float m_Gravity{ -60.f }; // new stull it is made at compile time
};

