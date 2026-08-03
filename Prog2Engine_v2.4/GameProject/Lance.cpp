#include "pch.h"
#include "Lance.h"
#include "utils.h"
#include "TextureManager.h"

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
	m_pTexture = TextureManager::GetInstance().GetTexture("Lance.png");
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

