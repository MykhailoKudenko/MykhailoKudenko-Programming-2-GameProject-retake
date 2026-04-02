#pragma once
#include <vector>
class Enemy
{
public:
	Enemy(Rectf Start);


	

	virtual void Update(float elapsedSec) = 0;
	virtual void Draw() const = 0;

	void DrawCollider() const;

	Rectf GetHitbox() const;

	void Kill();
	bool isDead() const;
	void TakeDamage();
	
protected:
	Rectf m_Collider;
	float m_Speed = 0;
	Vector2f m_Velocity{0,0};

	int m_health{ 1 };

	bool m_IsDead = false;
};

