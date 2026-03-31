#pragma once
#include "Enemy.h"
#include "Texture.h"

enum class FlyingKnightState
{
	Spawning,
	Flying
};

class FlyingKnight : public Enemy
{
public:
	FlyingKnight(Vector2f startPos, bool facingRight);

	static void InitializeAssets();
	static void FreeAssets();

	void Update(float elapsedSec) override;
	void Draw() const override;

private:
	bool m_IsFacingRight{ true };

	FlyingKnightState m_State{ FlyingKnightState::Spawning };

	float m_StateTime{ 0.f };
	float m_FlyTime{ 0.f };
	float m_StartY{};

	float m_Amplitude{ 20.f };
	float m_Frequency{ 2.f };
	float m_SpawnDuration{ 0.6f };

	static Texture* m_pTexture;
};