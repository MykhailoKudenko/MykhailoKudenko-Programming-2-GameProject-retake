#pragma once
#include "Projectile.h"
#include "Texture.h"

class DemonProjectile final: public Projectile
{
public:
	DemonProjectile(Vector2f pos, Vector2f direction);

	void Draw() const override;

private:
	const Texture* m_pTexture;

};



