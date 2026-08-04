#include "pch.h"
#include "Projectile.h"
#include "utils.h"

Projectile::Projectile(Rectf startPos) :m_Collider{ startPos }
{

}


void Projectile::Update(float elapsedSec)
{
	m_Collider.left += m_Speed.x * elapsedSec;
	m_Collider.bottom += m_Speed.y * elapsedSec;
}

void Projectile::Draw() const
{
	if (m_pTexture == nullptr)
	{
		return;
	}

	m_pTexture->Draw(
		Vector2f{ m_Collider.left, m_Collider.bottom },
		m_Speed.x <= 0
	);
}

Rectf Projectile::GetHitbox() const
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