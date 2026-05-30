#include "pch.h"
#include "Lance.h"
#include "utils.h"

Texture* Lance::m_pTexture{ nullptr };
int Lance::m_InstanceCount{ 0 };

Lance::Lance(Vector2f pos, bool isRight)
	: Projectile(Rectf{ pos.x, pos.y, 24.f, 5.f })
{
	if (isRight)
	{
		m_Speed.x = 100.f;
	}
	else
	{
		m_Speed.x = -100.f;
	}

	++m_InstanceCount;

	if (m_pTexture == nullptr)
	{
		m_pTexture = new Texture{ "Lance.png" };
	}
}
Lance::~Lance()
{
	--m_InstanceCount;

	if (m_InstanceCount <= 0)
	{
		delete m_pTexture;
		m_pTexture = nullptr;

		m_InstanceCount = 0;
	}
}
void Lance::Draw() const
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

