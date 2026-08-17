#pragma once
#include "TextureManager.h"

class Projectile
{
public:
	Projectile(const Rectf& startPos, const Vector2f& direction, float speed);
	virtual ~Projectile() = default;
	Projectile(const Projectile&) = delete;
	Projectile& operator=(const Projectile&) = delete;
	Projectile(Projectile&&) = delete;
	Projectile& operator=(Projectile&&) = delete;

	virtual void Update(float elapsedSec);

	virtual void Draw() const = 0;
	const Rectf& GetHitbox() const;
	Vector2f GetCenterPosition() const;

	virtual void Kill();
	bool IsDead() const;

protected:
	Rectf m_Collider;
	Vector2f m_Direction;
	float m_Speed;
	bool m_IsDead;
	const Texture* m_pTexture;
};

