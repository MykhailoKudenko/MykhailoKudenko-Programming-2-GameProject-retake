#pragma once
#include <vector>
#include "utils.h"
#include "Texture.h"

enum class PickupType
{
	Lance,
	Knife,
	Torch,
	Doll,
	MoneyBag
};



class Drop final
{
public:
	Drop(const Vector2f& pos, PickupType type);
	~Drop();

	void Update(float elapsedSec);
	void Draw() const;

	Rectf GetHitbox() const;
	PickupType GetType() const;
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
	PickupType m_Type;
	bool m_IsDead{ false };

	// static textures
	static Texture* m_pLanceTexture;
	static Texture* m_pKnifeTexture;
	static Texture* m_pTorchTexture;
	static Texture* m_pDollTexture;
	static Texture* m_pMoneyBagTexture;
	static int m_InstanceCount;

	Texture* GetTexture() const;

	static constexpr float m_Gravity{ -60.f };
	State m_Mystate = State::Falling;

};