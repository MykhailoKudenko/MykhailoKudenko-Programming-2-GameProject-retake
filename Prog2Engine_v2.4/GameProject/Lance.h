#pragma once
#include "Projectile.h"
#include "Texture.h"

class Lance : public Projectile
{
public:
	Lance(Vector2f pos, bool isRight);
	~Lance() override;


	void Draw() const override;

private:
	static Texture* m_pTexture;
	static int m_InstanceCount;

};
