#pragma once
#include <vector>
#include "Texture.h"
#include "Effect.h"
class Enemy
{
public:
	Enemy(Rectf Start);
	virtual ~Enemy();
	//rule of 5
	Enemy(const Enemy&) = delete;
	Enemy& operator=(const Enemy&) = delete;
	Enemy(Enemy&&) = delete;
	Enemy& operator=(Enemy&&) = delete;

	virtual void Update(float elapsedSec) = 0;
	virtual void Draw() const = 0;

	Rectf GetHitbox() const;

	void Kill();
	bool isDead() const;
	void TakeDamage();
	Vector2f GetCenterPosition() const;

	virtual bool isSpawning() const;

	bool GetIsActive() const; 
	void SetIsActive(bool isActive);

	int GetScore() const;



	void SetBag(bool DoesHaveBag);
	bool GetBag() const;
	void DrawBag() const;

	Effect::EffectType GetEffectType() const;

	bool IsFacingRight() const;
	bool IsBoss() const;
protected:
	Rectf m_Collider;
	float m_Speed{ 0.f };
	Vector2f m_Velocity{ 0.f, 0.f };

	int m_health{ 1 };
	int m_score{ 100 };
	bool m_IsDead{ false };
	bool m_IsBoss{ false };
	bool m_IsActive{ false };
	bool m_DoesHaveBag{ false };

	bool m_IsFacingRight{ true };

	Effect::EffectType m_EffectType{ Effect::EffectType::Blood };

	static int m_EnemyInstanceCount;
	static Texture* m_pBagTexture;
};

