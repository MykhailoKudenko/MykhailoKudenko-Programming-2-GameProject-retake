#include "pch.h"
#include "Enemy.h"
#include "utils.h"

Enemy::Enemy(Rectf Start) :m_Collider{Start}
{

}

Rectf Enemy::GetHitbox() const
{
	return (m_Collider);
}

void Enemy::DrawCollider() const
{
	utils::SetColor(Color4f{ 0, 1, 0, 1 });
	utils::DrawRect(m_Collider);
}

void Enemy::TakeDamage()
{
	m_health--;
	if (m_health <= 0)
	{
		this->Kill();
	}
}

void Enemy::Kill()
{
	m_IsDead = true;
}
bool Enemy::isDead() const
{
	return(m_IsDead);
}
