#pragma once
#include <vector>

#include "Projectile.h"
#include "Texture.h"
#include "Animation.h"




class Torch final : public Projectile
{
public:
	Torch(Vector2f pos, bool isRight);

	void Update(float elapsedSec) override;

	void Draw() const override;

	void SetWorld(const std::vector<std::vector<Vector2f>>* vertices);
	void Kill() override;
private:

	enum class TorchStates
	{
		FlyingUp,
		FlyingDown,
		Burning
	};
	const std::vector<std::vector<Vector2f>>* m_pVertices{};

	const Texture* m_pTexture;
	Animation m_BurningGround;

	TorchStates m_MyState = TorchStates::FlyingUp;

	float m_TimeFliyngUp = 0.5f;
};

