#pragma once
#include "Projectile.h"
#include "Texture.h"

class PlantProjectile final : public Projectile
{
public:
	PlantProjectile(Vector2f pos, Vector2f direction);

	void Draw() const override;

private:
	const Texture* m_pTexture;
};

