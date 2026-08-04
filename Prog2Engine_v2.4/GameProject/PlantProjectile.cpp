#include "pch.h"
#include "PlantProjectile.h"

PlantProjectile::PlantProjectile(Vector2f pos, Vector2f direction)
	: Projectile(Rectf{ pos.x, pos.y, 9.f, 9.f })
{
	m_Speed.x = direction.x * 60;
	m_Speed.y = direction.y * 60;

	m_pTexture = TextureManager::GetInstance().GetTexture("PlantBullet.png");

}
