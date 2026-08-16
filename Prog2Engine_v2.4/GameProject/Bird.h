#pragma once
#include "Enemy.h"
#include "Animation.h"


class Bird final : public Enemy
{
public:
	Bird(const Vector2f& startPos, bool facingRight);

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

	virtual bool IsSpawning() const override;


private:
	enum class BirdState
	{
		Spawning,
		Flying
	};

	BirdState m_MyState;
	float m_StartY;
	float m_FlyTime;

	const float m_Amplitude;
	const float m_Frequency;

	Animation m_FlyAnimation;
	Animation m_SpawnAnimation;
};