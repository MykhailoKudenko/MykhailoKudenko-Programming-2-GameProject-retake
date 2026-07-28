#pragma once
class Projectile
{
public:
	Projectile(Rectf Start);
	virtual ~Projectile();

	//rule of 5
	Projectile(const Projectile&) = delete;
	Projectile& operator=(const Projectile&) = delete;
	Projectile(Projectile&&) = delete;
	Projectile& operator=(Projectile&&) = delete;


	void virtual Update(float elapsedSec);

	void virtual Draw() const = 0;
	Rectf GetHitbox() const;
	Vector2f GetCenterPosition() const;

	void virtual Kill();
	bool isDead() const;

protected:
	Vector2f m_Speed{ 0,0 };
	Rectf m_Collider;

	bool m_IsDead = false;
};

