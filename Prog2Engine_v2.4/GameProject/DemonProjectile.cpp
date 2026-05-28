#include "pch.h"
#include "DemonProjectile.h"
#include "utils.h"

Texture* DemonProjectile::m_pTexture{ nullptr };
int DemonProjectile::m_InstanceCount{ 0 };

DemonProjectile::DemonProjectile(Vector2f pos, Vector2f Direction)
	: Projectile(Rectf{ pos.x, pos.y, 9.f, 9.f })
{
	m_Speed.x = Direction.x * 60;
	m_Speed.y = Direction.y * 60;

	++m_InstanceCount;

	if (m_pTexture == nullptr)
	{
		m_pTexture = new Texture{ "DemonBullet.png" };
	}
}

DemonProjectile::~DemonProjectile()
{
	--m_InstanceCount;

	if (m_InstanceCount <= 0)
	{
		delete m_pTexture;
		m_pTexture = nullptr;

		m_InstanceCount = 0;
	}
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