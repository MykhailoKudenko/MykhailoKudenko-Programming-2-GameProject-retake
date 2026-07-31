#pragma once
#include "Projectile.h"
#include "Texture.h"

class DemonProjectile : public Projectile
{
public:
	DemonProjectile(Vector2f pos, Vector2f direction);
	~DemonProjectile() override;

	void Draw() const override;

private:
	static Texture* m_pTexture;
	static int m_InstanceCount;

};



