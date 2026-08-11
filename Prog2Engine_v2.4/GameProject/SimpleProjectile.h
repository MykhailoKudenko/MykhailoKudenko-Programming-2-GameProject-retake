#pragma once
#include "Projectile.h"

class SimpleProjectile final : public Projectile
{
public:

    enum class SimpleProjectileType
    {
        Lance,
        Knife,
        Plant,
        Demon,
        Troll,
    };
    SimpleProjectile(Vector2f pos, Vector2f direction, SimpleProjectileType type);

    virtual void Draw() const override;

private:

};

