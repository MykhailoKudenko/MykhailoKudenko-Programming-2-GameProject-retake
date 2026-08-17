#include "pch.h"
#include "Torch.h"
#include "utils.h"

Torch::Torch(const Vector2f& pos, bool isFacingRight, const std::vector<std::vector<Vector2f>>* vertices)
	: Projectile(Rectf{ pos.x, pos.y,TextureManager::GetInstance().GetTexture("Torch.png")->GetWidth(),TextureManager::GetInstance().GetTexture("Torch.png")->GetHeight() }, Vector2f{ isFacingRight ? 0.848f : -0.848f , 0.530f }, 100),
	m_BurningGround{ "BurningGround.png", 2, 0.26f, false },
	m_TimeFlyingUp{ 0.5f },
	m_MyState{ TorchState::FlyingUp },
	m_pVertices{ vertices }
{
	m_pTexture = TextureManager::GetInstance().GetTexture("Torch.png");
}

void Torch::Draw() const
{
	if (m_MyState == TorchState::Burning)
	{
		m_BurningGround.Draw(m_Collider, m_Direction.x >= 0, false);
	}
	else
	{
		m_pTexture->Draw(m_Collider, m_Direction.x >= 0);
	}
}

void Torch::Update(float elapsedSec)
{
	if (m_pVertices == nullptr)
	{
		return;
	}

	switch (m_MyState)
	{
	case TorchState::FlyingUp:
		m_TimeFlyingUp -= elapsedSec;

		if (m_TimeFlyingUp <= 0)
		{
			m_MyState = TorchState::FlyingDown;
		}

		m_Collider.left += m_Direction.x * m_Speed * elapsedSec;
		m_Collider.bottom += m_Direction.y * m_Speed * elapsedSec;
		break;

	case TorchState::FlyingDown:

		if (utils::CheckSideCollision(*m_pVertices, m_Collider, utils::Side::bottom) == false)
		{
			m_Collider.left += m_Direction.x * m_Speed * elapsedSec;
			m_Collider.bottom -= m_Direction.y * m_Speed * elapsedSec;
		}
		else
		{
			m_MyState = TorchState::Burning;
		}
		break;

	case TorchState::Burning:
		m_BurningGround.Update(elapsedSec);
		if (m_BurningGround.IsFinished())
		{
			m_IsDead = true;
		}
		break;
	}
}


void Torch::Kill()
{
	if (m_MyState != TorchState::Burning)
	{
		m_IsDead = true;
	}
}

