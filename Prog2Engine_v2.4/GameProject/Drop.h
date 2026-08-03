#pragma once
#include <vector>
#include "utils.h"
#include "Texture.h"



class Drop final
{
public:

	enum class DropType
	{
		Lance,
		Knife,
		Torch,
		Doll,
		MoneyBag
	};


	Drop(const Vector2f& pos, DropType type);

	void Update(float elapsedSec);
	void Draw() const;

	Rectf GetHitbox() const;
	DropType GetType() const;
	bool IsDead() const;
	void Kill();

	void SetWorld(const std::vector<std::vector<Vector2f>>* vertices);

private:
	enum class State
	{
		Falling,
		NotFalling
	};

	const std::vector<std::vector<Vector2f>>* m_pVertices{ nullptr };

	Rectf m_Collider;
	DropType m_Type;
	bool m_IsDead{ false };

	// static textures
	const Texture* m_pLanceTexture;
	const Texture* m_pKnifeTexture;
	const Texture* m_pTorchTexture;
	const Texture* m_pDollTexture;
	const Texture* m_pMoneyBagTexture;

	const Texture* GetTexture() const;

	static constexpr float m_Gravity{ -60.f };
	State m_Mystate = State::Falling;

};