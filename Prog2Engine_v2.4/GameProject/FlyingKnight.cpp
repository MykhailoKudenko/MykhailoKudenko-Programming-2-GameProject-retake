#include "pch.h"
#include "FlyingKnight.h"
#include "utils.h"
#include <cmath>
#include "TextureManager.h"

FlyingKnight::FlyingKnight(Vector2f startPos, bool facingRight)
	: Enemy(Rectf{ startPos.x, startPos.y, 15.f, 24.f })
	, m_StartY{ startPos.y }
{

	m_IsFacingRight = facingRight;
	m_Speed = 35.f;


	if (!facingRight)
	{
		m_Speed *= -1.f;
	}

	m_pTexture = TextureManager::GetInstance().GetTexture("FlyingKnight.png");
	m_EffectType = Effect::EffectType::Fire;
}

void FlyingKnight::Update(float elapsedSec)
{
	
	m_FlyTime += elapsedSec;

	m_Collider.left += m_Speed * elapsedSec;
	m_Collider.bottom = m_StartY + std::sin(m_FlyTime * m_Frequency) * m_Amplitude;
	
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

