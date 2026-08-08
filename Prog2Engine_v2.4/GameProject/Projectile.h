#pragma once
#include "TextureManager.h"

class Projectile
{
public:
	Projectile(Rectf startPos, Vector2f direction, float speed);
	virtual ~Projectile() = default;

	void virtual Update(float elapsedSec);

	void virtual Draw() const = 0;
	Rectf GetHitbox() const;
	Vector2f GetCenterPosition() const;

	void virtual Kill();
	bool IsDead() const;

protected:
	Rectf m_Collider;
	Vector2f m_Direction;
	float m_Speed;
	bool m_IsDead;
	const Texture* m_pTexture;
};

