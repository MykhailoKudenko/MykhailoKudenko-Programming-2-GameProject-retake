#include "pch.h"
#include "DemonProjectile.h"

DemonProjectile::DemonProjectile(Vector2f pos, Vector2f direction)
	: Projectile(Rectf{ pos.x, pos.y, 9.f, 9.f })
{
	m_Speed.x = direction.x * 60;
	m_Speed.y = direction.y * 60;

	m_pTexture = TextureManager::GetInstance().GetTexture("DemonBullet.png");
}