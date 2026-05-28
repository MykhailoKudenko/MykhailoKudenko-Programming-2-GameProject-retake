#pragma once
#include "Projectile.h"
#include "Texture.h"

class Knife : public Projectile
{
public:
	Knife(Vector2f pos, bool isRight);
	~Knife() override;


	void Draw() const override;


private:
	static Texture* m_pTexture;
	static int m_InstanceCount;

};

