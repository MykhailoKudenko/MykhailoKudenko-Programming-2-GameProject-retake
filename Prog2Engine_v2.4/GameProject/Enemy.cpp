#include "pch.h"
#include "Enemy.h"
#include "utils.h"



Enemy::Enemy(const Rectf& startPos, float speed, bool isFacingRight, int score, bool doesHaveBag, Effect::EffectType deathEffect, SoundManager::SFX soundType, bool isBoss, int health)
	: m_Collider{ startPos },
	m_Speed{ speed },
	m_Velocity{ Vector2f{0, 0} },
	m_IsFacingRight{ isFacingRight },
	m_Health{ health },
	m_Score{ score },
	m_IsBoss{ isBoss },
	m_IsActive{ false},
	m_DoesHaveBag{ doesHaveBag },
	m_IsDead{ false },
	m_EffectType{ deathEffect },
	m_SoundType{ soundType },
	m_pBagTexture{ TextureManager::GetInstance().GetTexture("Bag.png") }
{
}



Rectf Enemy::GetHitbox() const
{
	return m_Collider;
}


void Enemy::TakeDamage()
{
	m_Health--;
	if (m_Health <= 0)
	{
		Kill();
	}
}

void Enemy::Kill()
{
	m_IsDead = true;
}
bool Enemy::IsDead() const
{
	return m_IsDead;
}
Vector2f Enemy::GetCenterPosition() const
{
	return Vector2f{ m_Collider.left + m_Collider.width / 2, m_Collider.bottom + m_Collider.height / 2 };
}


bool Enemy::IsSpawning() const
{
	return false;
}

bool Enemy::GetIsActive() const
{
	return m_IsActive;
}

void Enemy::SetIsActive(bool isActive)
{
	m_IsActive = isActive;
}

int Enemy::GetScore() const
{
	return m_Score;
}


void Enemy::SetBag(bool doesHaveBag)
{
	m_DoesHaveBag = doesHaveBag;
}
bool Enemy::HasBag() const
{
	return m_DoesHaveBag;
}
void Enemy::DrawBag() const
{
	if (!m_DoesHaveBag)
	{
		return;
	}
	m_pBagTexture->Draw(
		Vector2f{ m_Collider.left, m_Collider.bottom },
		m_IsFacingRight
	);
}

Effect::EffectType Enemy::GetEffectType() const
{
	return m_EffectType;
}
SoundManager::SFX Enemy::GetSoundType() const
{
	return m_SoundType;
}


bool Enemy::IsFacingRight() const
{
	return m_IsFacingRight;
}

bool Enemy::IsBoss() const
{
	return m_IsBoss;
}

