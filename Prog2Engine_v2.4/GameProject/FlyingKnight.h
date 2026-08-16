#pragma once
#include "Enemy.h"
#include "Texture.h"



class FlyingKnight final : public Enemy
{
public:
	FlyingKnight(const Vector2f& startPos, bool facingRight);

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

private:

	float m_StartY;
	float m_FlyTime;

	float m_Amplitude;
	float m_Frequency;

	const Texture* m_pTexture;
};