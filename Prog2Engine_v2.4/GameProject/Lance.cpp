#include "pch.h"
#include "Lance.h"

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