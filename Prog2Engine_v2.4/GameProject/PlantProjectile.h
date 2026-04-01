#pragma once
#include "Projectile.h"
#include "Texture.h"

class PlantProjectile : public Projectile
{
public:
	PlantProjectile(Vector2f pos, Vector2f Direction);
	virtual ~PlantProjectile() = default;

	void Draw() const override;

	static void InitializeAssets();
	static void FreeAssets();

private:
	static Texture* m_pTexture;
};

