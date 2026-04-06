#include "pch.h"
#include "Projectile.h"
#include "utils.h"

Projectile::Projectile(Rectf Start) :m_Collider{ Start }
{

}

void Projectile::Update(float elapsedSec)
{
	m_Collider.left += m_Speed.x * elapsedSec;
	m_Collider.bottom += m_Speed.y * elapsedSec;
}



Rectf Projectile::GetHitbox() const
{
	return( m_Collider );
}

void Projectile::Kill()
{
	m_IsDead = true;
}
bool Projectile::isDead()
{
	return(m_IsDead);
}