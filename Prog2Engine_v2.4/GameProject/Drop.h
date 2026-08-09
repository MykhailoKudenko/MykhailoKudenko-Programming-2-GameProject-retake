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


	Drop(const Vector2f& pos, DropType type, const std::vector<std::vector<Vector2f>>* vertices);

	void Update(float elapsedSec);
	void Draw() const;

	Rectf GetHitbox() const;
	DropType GetType() const;
	bool IsDead() const;
	void Kill();

private:
	enum class State
	{
		Falling,
		NotFalling
	};

	const std::vector<std::vector<Vector2f>>* m_pVertices;

	Rectf m_Collider;
	DropType m_Type;
	bool m_IsDead{ false };

	const Texture* m_pTexture{ nullptr };

	State m_Mystate = State::Falling;

};