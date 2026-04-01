#include "pch.h"
#include "PlantProjectile.h"
#include "utils.h"

Texture* PlantProjectile::m_pTexture{ nullptr };

PlantProjectile::PlantProjectile(Vector2f pos, Vector2f Direction)
	: Projectile(Rectf{ pos.x, pos.y, 10.f, 10.f })
{
	m_Speed.x = Direction.x * 60;
	m_Speed.y = Direction.y * 60;
}

void PlantProjectile::Draw() const
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

void PlantProjectile::InitializeAssets()
{
	if (m_pTexture == nullptr)
	{
		m_pTexture = new Texture{ "PlantBullet.png" };
	}
}

void PlantProjectile::FreeAssets()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}