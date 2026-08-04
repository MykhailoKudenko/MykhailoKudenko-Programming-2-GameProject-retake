
#include "pch.h"
#include "Knife.h"
#include "pch.h"
#include "Knife.h"

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

	m_pTexture = TextureManager::GetInstance().GetTexture("Knife.png");

}

