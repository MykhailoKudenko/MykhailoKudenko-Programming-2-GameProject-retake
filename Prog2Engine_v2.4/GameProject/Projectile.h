#pragma once
class Projectile
{
public:
	Projectile(Rectf startPos);
	virtual ~Projectile() = default;



	void virtual Update(float elapsedSec);

	void virtual Draw() const = 0;
	Rectf GetHitbox() const;
	Vector2f GetCenterPosition() const;

	void virtual Kill();
	bool IsDead() const;

protected:
	Vector2f m_Speed{ 0,0 };
	Rectf m_Collider;

	bool m_IsDead = false;
};

