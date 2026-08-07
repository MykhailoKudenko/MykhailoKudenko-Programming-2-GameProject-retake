#pragma once
#include "Enemy.h"
#include "Animation.h"


class Bird final : public Enemy
{
public:
	Bird(Vector2f startPos, bool facingRight);

	void Update(float elapsedSec) override;
	void Draw() const override;

	bool IsSpawning() const;


private:
	enum class BirdState
	{
		Spawning,
		Flying
	};

	BirdState m_State{ BirdState::Spawning };
	float m_FlyTime{ 0.f };
	float m_StartY{};

	float m_Amplitude{ 8.f };
	float m_Frequency{ 2.f };

	Animation m_pFlyAnimation;
	Animation m_pSpawnAnimation;
};