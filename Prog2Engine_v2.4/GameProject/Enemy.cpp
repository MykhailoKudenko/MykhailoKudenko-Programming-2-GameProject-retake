#include "pch.h"
#include "Enemy.h"

Enemy::Enemy(Rectf Start) :m_Collider{Start}
{

}

Rectf Enemy::GetHitbox() const
{
	return (m_Collider);
}

void Enemy::Kill()
{
	m_IsDead = true;
}
bool Enemy::isDead()
{
	return(m_IsDead);
}
