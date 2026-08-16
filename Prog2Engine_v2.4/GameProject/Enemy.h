#pragma once
#include <vector>
#include "Texture.h"
#include "TextureManager.h"
#include "SoundManager.h"

#include "Effect.h"
class Enemy
{
public:
	Enemy(const Rectf& startPos, float speed, bool isFacingRight, int score = 100, bool doesHaveBag = false, Effect::EffectType deathEffect = Effect::EffectType::Blood, SoundManager::SFX soundType = SoundManager::SFX::None, bool isBoss = false, int health = 1);
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



	void SetBag(bool doesHaveBag);
	bool HasBag() const;
	void DrawBag() const;
	Effect::EffectType GetEffectType() const;
	SoundManager::SFX GetSoundType() const;

	bool IsFacingRight() const;
	bool IsBoss() const;

protected:
	Rectf m_Collider;
	float m_Speed;
	Vector2f m_Velocity;
	bool m_IsFacingRight;

	int m_Health;
	int m_Score;
	bool m_IsBoss;
	bool m_IsActive;
	bool m_DoesHaveBag;
	bool m_IsDead;


	Effect::EffectType m_EffectType;
	SoundManager::SFX m_SoundType;

	const Texture* m_pBagTexture;

};

