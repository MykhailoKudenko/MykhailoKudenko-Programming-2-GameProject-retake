#pragma once
#include <vector>


#include "Player.h"
#include "Level.h"

#include "Bird.h"
#include "Zombie.h"
#include "FlyingKnight.h"
#include "Ghost.h"
#include "Plant.h"

#include "Demon.h"
#include "Troll.h"

#include "Lance.h"
#include "Knife.h"
#include "Torch.h"

#include "PlantProjectile.h"
#include "DemonProjectile.h"


#include "Drop.h"


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
    void AddPlant(const Vector2f& spawnPos);

    void AddDemon(const Vector2f& spawnPos);
    void AddTroll(const Vector2f& spawnPos);

    void SpawnLance(const Vector2f& pos, bool isRight);
    void SpawnKnife(const Vector2f& pos, bool isRight);
    void SpawnTourch(const Vector2f& pos, bool isRight);

    void SpawnPlantProjectile(const Vector2f& pos, const Vector2f& direction);
    void SpawnDemonProjectile(const Vector2f& pos, const Vector2f& direction);


    void AddDrop(const Vector2f& pos, PickupType type);

private:
    void RemoveDeadEntities();

    const Level* m_pLevel{ nullptr };

    std::vector<Enemy*> m_Enemies;
    std::vector<Projectile*> m_PlayerProjectiles;
    std::vector<Projectile*> m_EnemyProjectiles;
    std::vector<Drop*> m_Drops;
};