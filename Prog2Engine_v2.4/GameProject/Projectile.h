#pragma once
class Projectile
{
public:
	Projectile(Rectf Start);

	void virtual Update(float elapsedSec);

	void virtual Draw() const = 0;
	Rectf GetHitbox() const;

	void virtual Kill();
	bool isDead();

protected:
	Vector2f m_Speed{ 0,0 };
	Rectf m_Collider;

	bool m_IsDead = false;
};

