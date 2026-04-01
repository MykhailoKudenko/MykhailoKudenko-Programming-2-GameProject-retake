#pragma once
#include <vector>

#include "Projectile.h"
#include "Texture.h"
#include "Animation.h"


enum class TourchStates
{
	FlyingUp,
	FlyingDown,
	Burning
};

class Torch : public Projectile
{
public:
	Torch(Vector2f pos, bool isRight);
	virtual ~Torch() = default;

	void Update(float elapsedSec) override;

	void Draw() const override;

	static void InitializeAssets();
	static void FreeAssets();

	void SetWorld(const std::vector<std::vector<Vector2f>>* vertices);
	void Kill() override;
private:

	bool CheckBottomCollision();

	const std::vector<std::vector<Vector2f>>* m_pVertices{};

	static Texture* m_pTexture;
	static Animation* m_BurningGround;

	float m_AnimTime{ 0.f };

	TourchStates m_MyState = TourchStates::FlyingUp;

	float TimeFliyngUp = 0.5f;
};

