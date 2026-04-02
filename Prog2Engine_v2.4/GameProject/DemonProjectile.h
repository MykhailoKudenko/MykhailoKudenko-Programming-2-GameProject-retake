#pragma once
#include "Projectile.h"
#include "Texture.h"

class DemonProjectile : public Projectile
{
public:
	DemonProjectile(Vector2f pos, Vector2f Direction);
	virtual ~DemonProjectile() = default;

	void Draw() const override;

	static void InitializeAssets();
	static void FreeAssets();

private:
	static Texture* m_pTexture;
};



