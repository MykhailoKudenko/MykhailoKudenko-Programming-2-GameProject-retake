#pragma once
#include "Enemy.h"
#include "Animation.h"


class Bird final : public Enemy
{
public:
	Bird(Vector2f startPos, bool facingRight);

	void Update(float elapsedSec) override;
	void Draw() const override;

	bool IsSpawning() const override;


private:
	enum class BirdState
	{
		Spawning,
		Flying
	};

	BirdState m_State;
	float m_StartY;
	float m_FlyTime;

	float m_Amplitude;
	float m_Frequency;

	Animation m_FlyAnimation;
	Animation m_SpawnAnimation;
};