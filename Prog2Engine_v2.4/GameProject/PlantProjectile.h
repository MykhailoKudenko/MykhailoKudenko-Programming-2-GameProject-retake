#pragma once
#include "Projectile.h"
#include "Texture.h"

class PlantProjectile : public Projectile
{
public:
	PlantProjectile(Vector2f pos, Vector2f direction);
	~PlantProjectile() override;

	void Draw() const override;

private:
	static Texture* m_pTexture;
	static int m_InstanceCount;

};

