#pragma once
#include "Projectile.h"
#include "Texture.h"

class Knife : public Projectile
{
public:
	Knife(Vector2f pos, bool isRight);
	virtual ~Knife() = default;

	void Draw() const override;

	static void InitializeAssets();
	static void FreeAssets();

private:
	static Texture* m_pTexture;
};

