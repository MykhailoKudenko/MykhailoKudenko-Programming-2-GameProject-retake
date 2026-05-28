#pragma once
#include <vector>


#include "Player.h"
#include "Level.h"
#include "SoundManager.h"

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
    //rule of 5
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;
    EntityManager(EntityManager&&) = delete;
    EntityManager& operator=(EntityManager&&) = delete;

    void SetLevel(Level* pLevel);
    void SetPlayer(Player* pPlayer);
    void SetSoundManager(SoundManager* pSoundManager);

    Vector2f GetPlayerPosition() const;

    void Update(float elapsedSec);
    void Draw(bool isDebug) const;

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

    void SpawnPointEnemies();
    void SpawnPointDrops();

    void SpawnEffect(const Vector2f& pos, Effect::EffectType type, bool isMirrored = false);
  
private:

    void SpawnEnemyByType(Level::EnemyType type, const Vector2f& pos, bool startsFacingRight);
    bool FindGroundBelow(const Vector2f& pos, float& outGroundY) const;
    void SpawnAreaEnemies(float elapsedSec);

    void RemoveDeadEntities();
    void DebugSpawnDraw() const;

    bool RollBagDrop() const;
    PickupType GetRandomBagDrop() const;

    Player* m_pPlayer{ nullptr };
    Level* m_pLevel{ nullptr };

    SoundManager* m_pSoundManager{ nullptr };

    std::vector<Enemy*> m_Enemies;
    std::vector<Projectile*> m_PlayerProjectiles;
    std::vector<Projectile*> m_EnemyProjectiles;
    std::vector<Drop*> m_Drops;
    std::vector<Effect*> m_Effects;

    float UpdateLenth{ 256 / 2.f + 50 };
    float DeathLenth{ 256 / 2.f + 150 };

    float xSpawnLenth{256/2.f + 50};
    float yMinSpawnForAir{ 37.f };
    float yMaxHeight{ 200.f};
    float m_AreaSpawnInterval{ 1.f };
    //non unique sounds/ group sounds
    bool m_IsGhostSoundPlaying{ false };
    bool m_IsFlyingKnightSoundPlaying{ false };

    float m_GhostSoundTimer{ 0.f };
    float m_FlyingKnightSoundTimer{ 0.f };

    const float m_GhostSoundCooldown{ 2.0f };
    const float m_FlyingKnightSoundCooldown{ 2.0f };
};