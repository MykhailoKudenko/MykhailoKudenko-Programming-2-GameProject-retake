
#include "pch.h"
#include "Knife.h"
#include "utils.h"

Texture* Knife::m_pTexture{ nullptr };
int Knife::m_InstanceCount{ 0 };

Knife::Knife(Vector2f pos, bool isRight)
	: Projectile(Rectf{ pos.x, pos.y, 12.f, 7.f })
{
	if (isRight)
	{
		m_Speed.x = 120.f;
	}
	else
	{
		m_Speed.x = -120.f;
	}

	++m_InstanceCount;

	if (m_pTexture == nullptr)
	{
		m_pTexture = new Texture{ "Knife.png" };
	}
}
Knife::~Knife()
{
	--m_InstanceCount;

	if (m_InstanceCount <= 0)
	{
		delete m_pTexture;
		m_pTexture = nullptr;

		m_InstanceCount = 0;
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

