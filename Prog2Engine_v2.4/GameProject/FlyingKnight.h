#pragma once
#include "Enemy.h"
#include "Texture.h"



class FlyingKnight final : public Enemy
{
public:
	FlyingKnight(Vector2f startPos, bool facingRight);

	void Update(float elapsedSec) override;
	void Draw() const override;

private:

	float m_StateTime{ 0.f };
	float m_FlyTime{ 0.f };
	float m_StartY{};

	float m_Amplitude{ 50.f };
	float m_Frequency{ 2.f };

	const Texture* m_pTexture;
};