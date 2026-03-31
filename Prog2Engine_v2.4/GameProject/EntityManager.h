#pragma once
#include <vector>

#include "Lance.h"
#include "Player.h"
#include "Level.h"

#include "Bird.h"
#include "Zombie.h"
#include "FlyingKnight.h"
#include "Ghost.h"

class EntityManager
{
public:
    EntityManager();
    ~EntityManager();

    void SetLevel(const Level* pLevel);

    void Update(float elapsedSec, Player& player);
    void Draw() const;

    void AddZombie(const Vector2f& SpawnPos, bool startsFacingRight);
    void AddBird(const Vector2f& SpawnPos, bool startsFacingRight);
    void AddFlyingKnight(const Vector2f& SpawnPos, bool startsFacingRight);
    void AddGhost(const Vector2f& spawnPos, bool startsFacingRight);

    void SpawnProjectile(const Projectile& projectile);


private:
    void RemoveDeadEntities();

    const Level* m_pLevel{ nullptr };

    std::vector<Enemy*> m_Enemies;
    std::vector<Projectile> m_Projectiles;
};