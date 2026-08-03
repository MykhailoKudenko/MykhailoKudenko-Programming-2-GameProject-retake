#pragma once
#include "Projectile.h"
#include "Texture.h"

class Lance final : public Projectile
{
public:
	Lance(Vector2f pos, bool isRight);

	void Draw() const override;

private:
	const Texture* m_pTexture;

};
