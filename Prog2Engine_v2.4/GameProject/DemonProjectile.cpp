#include "pch.h"
#include "DemonProjectile.h"
#include "utils.h"

Texture* DemonProjectile::m_pTexture{ nullptr };

DemonProjectile::DemonProjectile(Vector2f pos, Vector2f Direction)
	: Projectile(Rectf{ pos.x, pos.y, 10.f, 10.f })
{
	m_Speed.x = Direction.x * 60;
	m_Speed.y = Direction.y * 60;
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

void DemonProjectile::InitializeAssets()
{
	if (m_pTexture == nullptr)
	{
		m_pTexture = new Texture{ "DemonBullet.png" };
	}
}

void DemonProjectile::FreeAssets()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}