#include "pch.h"
#include "FlyingKnight.h"
#include "utils.h"
#include <cmath>
#include "TextureManager.h"

FlyingKnight::FlyingKnight(Vector2f startPos, bool facingRight)
	: Enemy(Rectf{ startPos.x, startPos.y, 15.f, 24.f }, facingRight ? 35.f : -35.f, facingRight, 100, false, Effect::EffectType::Fire, SoundManager::SFX::FlyingKnight),
	m_StartY{ startPos.y },
	m_StateTime{ 0.f },
	m_FlyTime{ 0.f },
	m_Amplitude{ 50.f },
	m_Frequency{ 2.f },
	m_pTexture{ TextureManager::GetInstance().GetTexture("FlyingKnight.png") }
{
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

