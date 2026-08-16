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
    SimpleProjectile(const Vector2f& pos, const Vector2f& direction, SimpleProjectileType type);

    virtual void Draw() const override;

private:

};

