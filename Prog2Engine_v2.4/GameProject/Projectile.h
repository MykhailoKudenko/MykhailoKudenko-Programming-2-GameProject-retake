#pragma once
class Projectile
{
public:
	Projectile(Rectf Start);

	void Update(float elapsedSec);

	void Draw() const;

	Rectf GetHitbox() const;

	void Kill();
	bool isDead();

protected:
	Vector2f m_Speed{ 0,0 };
	Rectf m_Collider;

	bool m_IsDead = false;
};

