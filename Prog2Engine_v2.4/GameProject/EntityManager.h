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


#include "SimpleProjectile.h"
#include "Torch.h"

#include "Drop.h"

 
class EntityManager final
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

    void AddZombie(const Vector2f& spawnPos, bool startsFacingRight);
    void AddBird(const Vector2f& spawnPos, bool startsFacingRight);
    void AddFlyingKnight(const Vector2f& spawnPos, bool startsFacingRight);
    void AddGhost(const Vector2f& spawnPos, bool startsFacingRight);
    void AddPlant(const Vector2f& spawnPos);

    void AddDemon(const Vector2f& spawnPos);
    void AddTroll(const Vector2f& spawnPos);


    void SpawnPlayerWeapon(const Vector2f& pos, bool isRight, Player::PlayerWeapon weapon);
    void SpawnEnemyProjectile(const Vector2f& pos, const Vector2f& direction,SimpleProjectile::SimpleProjectileType type);


    void AddDrop(const Vector2f& pos, Drop::DropType type);

    void SpawnPointEnemies();
    void SpawnPointDrops();

    void SpawnEffect(const Vector2f& pos, Effect::EffectType type, bool isMirrored = false);
  
private:
    void SpawnTorch(const Vector2f& pos, bool isRight);
    void KillProjectilesOutsideSpawnArea();

    void SpawnEnemyByType(Level::EnemyType type, const Vector2f& pos, bool startsFacingRight);
    bool FindGroundBelow(const Vector2f& pos, float& outGroundY) const;
    void SpawnAreaEnemies(float elapsedSec);

    void RemoveDeadEntities();
    void DebugSpawnDraw() const;

    bool RollBagDrop() const;
    Drop::DropType GetRandomBagDrop() const;

    Player* m_pPlayer;
    Level* m_pLevel;

    SoundManager* m_pSoundManager;

    std::vector<Enemy*> m_pEnemies;
    std::vector<Projectile*> m_pPlayerProjectiles;
    std::vector<Projectile*> m_pEnemyProjectiles;
    std::vector<Drop*> m_pDrops;
    std::vector<Effect*> m_pEffects;

    float m_UpdateLenth;
    float m_DeathLenth;

    float m_XSpawnLenth;
    float m_YMinSpawnForAir;
    float m_YMaxHeight;
    //non unique sounds/ group sounds
    bool m_IsGhostSoundPlaying;
    bool m_IsFlyingKnightSoundPlaying;

    float m_GhostSoundTimer;
    float m_FlyingKnightSoundTimer;

    const float m_GhostSoundCooldown;
    const float m_FlyingKnightSoundCooldown;
};