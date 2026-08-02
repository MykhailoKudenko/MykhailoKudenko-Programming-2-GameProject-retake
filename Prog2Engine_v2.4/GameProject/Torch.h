#pragma once
#include <vector>

#include "Projectile.h"
#include "Texture.h"
#include "Animation.h"




class Torch final : public Projectile
{
public:
	Torch(Vector2f pos, bool isRight);
	~Torch() override;

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

	static Texture* m_pTexture;
	static Animation* m_BurningGround;
	static int m_InstanceCount;

	float m_AnimTime{ 0.f };

	TorchStates m_MyState = TorchStates::FlyingUp;

	float m_TimeFliyngUp = 0.5f;
};

