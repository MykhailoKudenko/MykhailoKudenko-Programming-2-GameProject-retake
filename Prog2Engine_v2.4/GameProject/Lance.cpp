#include "pch.h"
#include "Lance.h"

Lance::Lance(Vector2f pos, bool IsRight) : Projectile(Rectf{ pos.x, pos.y, 20, 10 })
{
	if (IsRight)
	{
		m_Speed.x = 40;
	}
	else
	{
		m_Speed.x = -40;
	}
}