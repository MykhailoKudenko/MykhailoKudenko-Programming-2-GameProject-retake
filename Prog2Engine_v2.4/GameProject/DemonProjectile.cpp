#include "pch.h"
#include "DemonProjectile.h"
#include "utils.h"
#include "TextureManager.h"

DemonProjectile::DemonProjectile(Vector2f pos, Vector2f direction)
	: Projectile(Rectf{ pos.x, pos.y, 9.f, 9.f })
{
	m_Speed.x = direction.x * 60;
	m_Speed.y = direction.y * 60;

	m_pTexture = TextureManager::GetInstance().GetTexture("DemonBullet.png");
}

void DemonProjectile::Draw() const
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