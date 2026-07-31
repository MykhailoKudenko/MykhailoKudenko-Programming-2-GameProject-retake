#pragma once
#include <vector>
#include "Texture.h"
#include "TextureManager.h"

#include "Effect.h"
class Enemy
{
public:
	Enemy(Rectf startPos);
	virtual ~Enemy() = default;

	Enemy(const Enemy&) = delete;
	Enemy& operator=(const Enemy&) = delete;
	Enemy(Enemy&&) = delete;
	Enemy& operator=(Enemy&&) = delete;


	virtual void Update(float elapsedSec) = 0;
	virtual void Draw() const = 0;

	Rectf GetHitbox() const;

	void Kill();
	bool IsDead() const;
	void TakeDamage();
	Vector2f GetCenterPosition() const;

	virtual bool IsSpawning() const;

	bool GetIsActive() const; 
	void SetIsActive(bool isActive);

	int GetScore() const;



	void SetBag(bool DoesHaveBag);
	bool HasBag() const;
	void DrawBag() const;
	Effect::EffectType GetEffectType() const;

	bool IsFacingRight() const;
	bool IsBoss() const;
protected:
	Rectf m_Collider;
	float m_Speed{ 0.f };
	Vector2f m_Velocity{ 0.f, 0.f };

	int m_Health{ 1 };
	int m_Score{ 100 };
	bool m_IsDead{ false };
	bool m_IsBoss{ false };
	bool m_IsActive{ false };
	bool m_DoesHaveBag{ false };

	bool m_IsFacingRight{ true };

	Effect::EffectType m_EffectType{ Effect::EffectType::Blood };


};

