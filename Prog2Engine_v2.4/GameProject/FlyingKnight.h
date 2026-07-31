#pragma once
#include "Enemy.h"
#include "Texture.h"



class FlyingKnight final : public Enemy
{
public:
	FlyingKnight(Vector2f startPos, bool facingRight);
	~FlyingKnight() override;

	void Update(float elapsedSec) override;
	void Draw() const override;

private:

	enum class FlyingKnightState
	{
		Spawning,
		Flying
	};

	FlyingKnightState m_State{ FlyingKnightState::Spawning };

	float m_StateTime{ 0.f };
	float m_FlyTime{ 0.f };
	float m_StartY{};

	float m_Amplitude{ 50.f };
	float m_Frequency{ 2.f };
	float m_SpawnDuration{ 0.6f };

	static Texture* m_pTexture;
	static int m_InstanceCount;
};