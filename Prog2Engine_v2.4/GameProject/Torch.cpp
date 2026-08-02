#include "pch.h"
#include "Torch.h"
#include "utils.h"

Texture* Torch::m_pTexture{ nullptr };
Animation* Torch::m_BurningGround{ nullptr };
int Torch::m_InstanceCount{ 0 };

Torch::Torch(Vector2f pos, bool isRight)
	: Projectile(Rectf{ pos.x, pos.y, 12.f, 8.f })
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

	++m_InstanceCount;

	if (m_pTexture == nullptr)
	{
		m_pTexture = new Texture{ "Torch.png" };
	}
	if (m_BurningGround == nullptr)
	{
		m_BurningGround = new Animation("BurningGround.png", 2, 0.26f, false);
	}
}
Torch::~Torch()
{
	--m_InstanceCount;

	if (m_InstanceCount <= 0)
	{
		delete m_pTexture;
		m_pTexture = nullptr;

		delete m_BurningGround;
		m_BurningGround = nullptr;

		m_InstanceCount = 0;
	}
}

void Torch::Draw() const
{
	if (m_MyState == TorchStates::Burning)
	{
		if (m_BurningGround != nullptr)
		{
			m_BurningGround->DrawAtTime(
				Rectf
				{
					m_Collider.left,
					m_Collider.bottom,
					m_BurningGround->GetFrameWidth(),
					m_BurningGround->GetFrameHeight()
				},
				m_AnimTime,
				m_Speed.x >= 0
			);
		}
	}
	else
	{
		if (m_pTexture != nullptr)
		{
			m_pTexture->Draw(
				Vector2f{ m_Collider.left, m_Collider.bottom },
				m_Speed.x >= 0
			);
		}
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
			m_AnimTime = 0.f;
		}
		break;

	case TorchStates::Burning:
		m_AnimTime += elapsedSec;

		if (m_BurningGround != nullptr && m_BurningGround->IsTimeFinished(m_AnimTime))
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