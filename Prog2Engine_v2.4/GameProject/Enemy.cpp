#include "pch.h"
#include "Enemy.h"
#include "utils.h"

Texture* Enemy::m_pBagTexture{ nullptr };
int Enemy::m_EnemyInstanceCount{ 0 };


Enemy::Enemy(Rectf Start)
	: m_Collider{ Start }
{
	++m_EnemyInstanceCount;

	if (m_pBagTexture == nullptr)
	{
		m_pBagTexture = new Texture("Bag.png");
	}
}

Enemy::~Enemy()
{
	--m_EnemyInstanceCount;

	if (m_EnemyInstanceCount <= 0)
	{
		delete m_pBagTexture;
		m_pBagTexture = nullptr;

		m_EnemyInstanceCount = 0;
	}
}

Rectf Enemy::GetHitbox() const
{
	return (m_Collider);
}


void Enemy::TakeDamage()
{
	m_health--;
	if (m_health <= 0)
	{
		this->Kill();
	}
}

void Enemy::Kill()
{
	m_IsDead = true;
}
bool Enemy::isDead() const
{
	return(m_IsDead);
}
Vector2f Enemy::GetCenterPosition() const
{
	return(Vector2f{ m_Collider.left + m_Collider.width / 2, m_Collider.bottom + m_Collider.height / 2 });
}


bool Enemy::isSpawning()
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

int Enemy::GetScore()
{
	return m_score;
}


void Enemy::SetBag(bool DoesHaveBag)
{
	m_DoesHaveBag = DoesHaveBag;
}
bool Enemy::GetBag() const
{
	return m_DoesHaveBag;
}
void Enemy::DrawBag() const
{
	if (!m_DoesHaveBag)
		return;

	m_pBagTexture->Draw(
		Vector2f{ m_Collider.left, m_Collider.bottom },
		m_IsFacingRight
	);
}

Effect::EffectType Enemy::GetEffectType()
{
	return m_EffectType;
}


bool Enemy::IsFacingRight() const
{
	return m_IsFacingRight;
}