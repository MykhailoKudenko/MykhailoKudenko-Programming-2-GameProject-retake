
#include "pch.h"
#include "Knife.h"
#include "utils.h"

Texture* Knife::m_pTexture{ nullptr };

Knife::Knife(Vector2f pos, bool isRight)
	: Projectile(Rectf{ pos.x, pos.y, 20.f, 10.f })
{
	if (isRight)
	{
		m_Speed.x = 80.f;
	}
	else
	{
		m_Speed.x = -80.f;
	}
}

void Knife::Draw() const
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

void Knife::InitializeAssets()
{
	if (m_pTexture == nullptr)
	{
		m_pTexture = new Texture{ "Knife.png" };
	}
}

void Knife::FreeAssets()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}