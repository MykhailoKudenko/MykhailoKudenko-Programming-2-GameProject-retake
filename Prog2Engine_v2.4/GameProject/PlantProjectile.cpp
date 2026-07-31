#include "pch.h"
#include "PlantProjectile.h"
#include "utils.h"

Texture* PlantProjectile::m_pTexture{ nullptr };
int PlantProjectile::m_InstanceCount{ 0 };

PlantProjectile::PlantProjectile(Vector2f pos, Vector2f direction)
	: Projectile(Rectf{ pos.x, pos.y, 9.f, 9.f })
{
	m_Speed.x = direction.x * 60;
	m_Speed.y = direction.y * 60;

	++m_InstanceCount;

	if (m_pTexture == nullptr)
	{
		m_pTexture = new Texture{ "PlantBullet.png" };
	}
}

PlantProjectile::~PlantProjectile()
{
	--m_InstanceCount;

	if (m_InstanceCount <= 0)
	{
		delete m_pTexture;
		m_pTexture = nullptr;

		m_InstanceCount = 0;
	}
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
