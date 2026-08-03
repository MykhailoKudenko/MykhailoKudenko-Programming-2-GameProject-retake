#include "pch.h"
#include "Torch.h"
#include "utils.h"
#include "TextureManager.h"

Torch::Torch(Vector2f pos, bool isRight)
	: Projectile(Rectf{ pos.x, pos.y, 12.f, 8.f }),
	m_BurningGround{ Animation("BurningGround.png", 2, 0.26f, false) }
{
	if (isRight)
	{
		m_Speed.x = 80.f;
	}
	else
	{
		m_Speed.x = -80.f;

	}
	m_Speed.y = 50;

	if (m_pTexture == nullptr)
	{
		m_pTexture = TextureManager::GetInstance().GetTexture("Torch.png");
	}

}

void Torch::Draw() const
{
	if (m_MyState == TorchStates::Burning)
	{
		m_BurningGround.Draw(m_Collider, m_Speed.x >= 0, false);
	}
	else
	{
		m_pTexture->Draw(Vector2f{ m_Collider.left, m_Collider.bottom }, m_Speed.x >= 0);
	}
}

void Torch::Update(float elapsedSec)
{
	switch (m_MyState)
	{
	case TorchStates::FlyingUp:
		m_TimeFliyngUp -= elapsedSec;

		if (m_TimeFliyngUp <= 0)
		{
			m_MyState = TorchStates::FlyingDown;
		}

		m_Collider.left += m_Speed.x * elapsedSec;
		m_Collider.bottom += m_Speed.y * elapsedSec;
		break;

	case TorchStates::FlyingDown:
		if (utils::CheckSideCollision(*m_pVertices, m_Collider, utils::Side::bottom) == false)
		{
			m_Collider.left += m_Speed.x * elapsedSec;
			m_Collider.bottom -= m_Speed.y * elapsedSec;
		}
		else
		{
			m_MyState = TorchStates::Burning;
		}
		break;

	case TorchStates::Burning:
		m_BurningGround.Update(elapsedSec);
		if (m_BurningGround.IsFinished());
		{
			m_IsDead = true;
		}
		break;
	}
}


void Torch::Kill()
{
	if (m_MyState != TorchStates::Burning)
	{
		m_IsDead = true;
	}
}

void Torch::SetWorld(const std::vector<std::vector<Vector2f>>* vertices)
{
	m_pVertices = vertices;
}