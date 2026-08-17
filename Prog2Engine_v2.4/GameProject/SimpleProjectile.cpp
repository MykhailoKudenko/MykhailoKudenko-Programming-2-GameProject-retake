#include "pch.h"
#include "SimpleProjectile.h"

namespace {
	const Texture* GetTexture(SimpleProjectile::SimpleProjectileType type)
	{
		switch (type)
		{
		case SimpleProjectile::SimpleProjectileType::Lance:
			return TextureManager::GetInstance().GetTexture("Lance.png");
		case SimpleProjectile::SimpleProjectileType::Knife:
			return TextureManager::GetInstance().GetTexture("Knife.png");
		case SimpleProjectile::SimpleProjectileType::Plant:
			return TextureManager::GetInstance().GetTexture("PlantBullet.png");
		case SimpleProjectile::SimpleProjectileType::Demon:
			return TextureManager::GetInstance().GetTexture("DemonBullet.png");
		case SimpleProjectile::SimpleProjectileType::Troll:
			return TextureManager::GetInstance().GetTexture("TrollBullet.png");
		default:
			return nullptr;
		}
	}

	float GetSpeed(SimpleProjectile::SimpleProjectileType type)
	{
		switch (type)
		{
		case SimpleProjectile::SimpleProjectileType::Lance:
			return 100.f;
		case SimpleProjectile::SimpleProjectileType::Knife:
			return 120.f;
		case SimpleProjectile::SimpleProjectileType::Plant:
		case SimpleProjectile::SimpleProjectileType::Demon:
		case SimpleProjectile::SimpleProjectileType::Troll:
			return 60.f;
		default:
			return 0;
		}
	}
}

SimpleProjectile::SimpleProjectile(const Vector2f& pos, const Vector2f& direction, SimpleProjectileType type)
	: Projectile(Rectf{ pos.x, pos.y,GetTexture(type)->GetWidth(),GetTexture(type)->GetHeight() }, direction, GetSpeed(type))
{
	m_pTexture = GetTexture(type);
}

void SimpleProjectile::Draw() const
{
	if (m_pTexture == nullptr)
	{
		return;
	}

	m_pTexture->Draw(
		m_Collider,
		m_Direction.x <= 0
	);
}


