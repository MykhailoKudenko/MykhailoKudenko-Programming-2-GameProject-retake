#pragma once
#include <vector>

#include "Projectile.h"
#include "Texture.h"
#include "Animation.h"




class Torch final : public Projectile
{
public:
	Torch(Vector2f pos, bool isFacingRight, const std::vector<std::vector<Vector2f>>* vertices);

	virtual void Update(float elapsedSec) override;

	virtual void Draw() const override;

	virtual void Kill() override;
private:

	enum class TorchStates
	{
		FlyingUp,
		FlyingDown,
		Burning
	};
	const std::vector<std::vector<Vector2f>>* m_pVertices;

	Animation m_BurningGround;

	TorchStates m_MyState;

	float m_TimeFlyingUp;
};

