#pragma once
#include "Enemy.h"
#include "Animation.h"

enum class BirdState
{
	Spawning,
	Flying
};

class Bird final : public Enemy
{
public:
	Bird(Vector2f startPos, bool facingRight);
	~Bird() override;


	void Update(float elapsedSec) override;
	void Draw() const override;

private:

	BirdState m_State{ BirdState::Spawning };

	float m_AnimTime{ 0.f };
	float m_FlyTime{ 0.f };
	float m_StartY{};

	float m_Amplitude{ 8.f };
	float m_Frequency{ 2.f };

	static Animation* m_pFlyAnimation;
	static Animation* m_pSpawnAnimation;
	static int m_InstanceCount;
};