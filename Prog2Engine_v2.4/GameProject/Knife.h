#pragma once
#include "Projectile.h"
#include "Texture.h"

class Knife final : public Projectile
{
public:
	Knife(Vector2f pos, bool isRight);

	void Draw() const override;

private:
	const Texture* m_pTexture;

};

