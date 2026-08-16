#include "pch.h"
#include "Projectile.h"
#include "utils.h"

Projectile::Projectile(const Rectf& startPos, const Vector2f& direction, float speed) :m_Collider{ startPos }, m_Direction{ direction }, m_Speed{ speed }, m_IsDead{false}, m_pTexture{nullptr}
{

}


void Projectile::Update(float elapsedSec)
{
	m_Collider.left += m_Direction.x * m_Speed * elapsedSec;
	m_Collider.bottom += m_Direction.y * m_Speed* elapsedSec;
}

const Rectf& Projectile::GetHitbox() const
{
	return m_Collider;
}

void Projectile::Kill()
{
	m_IsDead = true;
}
bool Projectile::IsDead() const
{
	return m_IsDead;
}

Vector2f Projectile::GetCenterPosition() const
{
	return Vector2f
	{
		m_Collider.left + m_Collider.width / 2.f,
		m_Collider.bottom + m_Collider.height / 2.f
	};
}