#include "pch.h"
#include "FlyingKnight.h"
#include "utils.h"
#include <cmath>

Texture* FlyingKnight::m_pTexture{ nullptr };

FlyingKnight::FlyingKnight(Vector2f startPos, bool facingRight)
	: Enemy(Rectf{ startPos.x, startPos.y, 15.f, 24.f })
	, m_State{ FlyingKnightState::Spawning }
	, m_StartY{ startPos.y }
{

	m_IsFacingRight = facingRight;
	m_Speed = 35.f;

	if (!facingRight)
	{
		m_Speed *= -1.f;
	}
}

void FlyingKnight::Update(float elapsedSec)
{
	m_StateTime += elapsedSec;

	if (m_State == FlyingKnightState::Spawning)
	{
		if (m_StateTime >= m_SpawnDuration)
		{
			m_State = FlyingKnightState::Flying;
			m_StateTime = 0.f;
			m_FlyTime = 0.f;
		}
		return;
	}

	if (m_State == FlyingKnightState::Flying)
	{
		m_FlyTime += elapsedSec;

		m_Collider.left += m_Speed * elapsedSec;
		m_Collider.bottom = m_StartY + std::sin(m_FlyTime * m_Frequency) * m_Amplitude;
	}
}

void FlyingKnight::Draw() const
{
	if (m_pTexture == nullptr)
	{
		utils::SetColor(Color4f{ 0.f, 0.f, 1.f, 1.f });
		utils::DrawRect(m_Collider);
		return;
	}

	m_pTexture->Draw(
		Vector2f{ m_Collider.left, m_Collider.bottom },
		m_IsFacingRight
	);
}

void FlyingKnight::InitializeAssets()
{
	if (m_pTexture == nullptr)
	{
		m_pTexture = new Texture("FlyingKnight.png");
	}
}

void FlyingKnight::FreeAssets()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}