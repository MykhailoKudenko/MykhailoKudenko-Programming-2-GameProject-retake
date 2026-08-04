#pragma once
#include "TextureManager.h"

class Projectile
{
public:
	Projectile(Rectf startPos);
	virtual ~Projectile() = default;

	void virtual Update(float elapsedSec);

	void virtual Draw() const;
	Rectf GetHitbox() const;
	Vector2f GetCenterPosition() const;

	void virtual Kill();
	bool IsDead() const;

protected:
	Vector2f m_Speed{ 0,0 };
	Rectf m_Collider;

	const Texture* m_pTexture{nullptr};

	bool m_IsDead = false;
};

