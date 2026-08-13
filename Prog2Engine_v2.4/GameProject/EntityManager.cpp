#include "pch.h"
#include "EntityManager.h"
#include "utils.h"
#include <cstdlib>
#include <cmath>

namespace {
    void DrawGreenRectIfDebug(const Rectf& rect, bool isDebug)
    {
        if (isDebug)
        {
            utils::SetColor(Color4f{ 0, 1, 0, 1 });
            utils::DrawRect(rect);
        }
    }
    bool RollBagDrop()
    {
        return std::rand() % 10 == 0;
    }

    Drop::DropType GetRandomBagDrop()
    {
        int randomIndex = std::rand() % 5;

        return static_cast<Drop::DropType>( randomIndex);
        switch (randomIndex)
        {
        case 0: return  Drop::DropType::Lance;
        case 1: return  Drop::DropType::Knife;
        case 2: return  Drop::DropType::Torch;
        case 3: return  Drop::DropType::Doll;
        case 4: return  Drop::DropType::MoneyBag;
        default: return  Drop::DropType::Doll;
        }
    }
}

EntityManager::EntityManager():

m_pPlayer{ nullptr },
m_pLevel{ nullptr },

m_pSoundManager{ nullptr },

m_pEnemies{ },
m_pPlayerProjectiles{ },
m_pEnemyProjectiles{ },
m_pDrops{ },
m_pEffects{ },

m_UpdateLenth{ 256 / 2.f + 50 },

m_XSpawnLenth{ 256 / 2.f + 50 },
m_YMinSpawnForAir{ 37.f },
m_YMaxHeight{ 200.f },

m_IsGhostSoundPlaying{ false },
m_IsFlyingKnightSoundPlaying{ false },

m_GhostSoundTimer{ 0.f },
m_FlyingKnightSoundTimer{ 0.f },

m_GhostSoundCooldown{ 2.0f },
m_FlyingKnightSoundCooldown{ 2.0f }
{
}

EntityManager::~EntityManager()
{
    for (Enemy* enemy : m_pEnemies)
    {
        delete enemy;
    }
    for (Projectile* proj : m_pPlayerProjectiles)
    {
        delete proj;
    }
    for (Projectile* proj : m_pEnemyProjectiles)
    {
        delete proj;
    }
    for (Drop* drop : m_pDrops)
    {
        delete drop;
    }
    for (Effect* effect : m_pEffects)
    {
        delete effect;
    }
}

void EntityManager::Update(float elapsedSec)
{
    if (m_pPlayer == nullptr || m_pLevel == nullptr)
    {
        return;
    }

    SpawnPointEnemies();
    SpawnPointDrops();
    SpawnAreaEnemies(elapsedSec);

    for (Effect* effect : m_pEffects)
    {
        effect->Update(elapsedSec);
    }

    Vector2f playerPos = m_pPlayer->GetCenterPosition();

    m_GhostSoundTimer -= elapsedSec;
    m_FlyingKnightSoundTimer -= elapsedSec;

    bool hasGhostInRange{ false };
    bool hasFlyingKnightInRange{ false };

    for (Enemy* enemy : m_pEnemies)
    {
        float dx = std::abs(enemy->GetCenterPosition().x - playerPos.x);


        if (dx <= m_UpdateLenth)
        {
            enemy->SetIsActive(true);
        }
        else
        {
            if (enemy->IsBoss() == false)
            {
                enemy->SetIsActive(false);
            }
        }

        if (enemy->GetIsActive())
        {
            if (enemy->GetSoundType() == SoundManager::SFX::Ghost)
            {
                hasGhostInRange = true;
            }

            if (enemy->GetSoundType() == SoundManager::SFX::FlyingKnight)
            {
                hasFlyingKnightInRange = true;
            }

            enemy->Update(elapsedSec);

            if (utils::IsOverlapping(enemy->GetHitbox(), m_pPlayer->GetHitbox()))
            {
                if (!enemy->IsSpawning())
                {
                    m_pPlayer->TakeDamage();
                }
            }
        }

        if (enemy->GetCenterPosition().y < -20.f)
        {
            enemy->Kill();
        }
    }

    if (hasGhostInRange && m_GhostSoundTimer <= 0.f)
    {
        if (m_pSoundManager != nullptr)
        {
            m_pSoundManager->PlayEffect(SoundManager::SFX::Ghost);
        }
        m_GhostSoundTimer = m_GhostSoundCooldown;
    }

    if (hasFlyingKnightInRange && m_FlyingKnightSoundTimer <= 0.f)
    {
        if (m_pSoundManager != nullptr)
        {
            m_pSoundManager->PlayEffect(SoundManager::SFX::FlyingKnight);
        }
        m_FlyingKnightSoundTimer = m_FlyingKnightSoundCooldown;
    }

    for (Projectile* proj : m_pPlayerProjectiles)
    {
        proj->Update(elapsedSec);
    }

    for (Projectile* proj : m_pEnemyProjectiles)
    {
        proj->Update(elapsedSec);
        if (utils::IsOverlapping(m_pPlayer->GetHitbox(), proj->GetHitbox()))
        {
            m_pPlayer->TakeDamage();
            proj->Kill();
        }
    }

    for (Drop* drop : m_pDrops)
    {
        drop->Update(elapsedSec);
        if (utils::IsOverlapping(m_pPlayer->GetHitbox(), drop->GetHitbox()))
        {
            if (m_pSoundManager != nullptr)
            {
                m_pSoundManager->PlayEffect(SoundManager::SFX::PickUp);
            }
            switch (drop->GetType())
            {
            case Drop::DropType::Lance:
                m_pPlayer->SetPlayerWeapon(Player::PlayerWeapon::Lance);
                break;
            case Drop::DropType::Knife:
                m_pPlayer->SetPlayerWeapon(Player::PlayerWeapon::Knife);
                break;
            case Drop::DropType::Torch:
                m_pPlayer->SetPlayerWeapon(Player::PlayerWeapon::Torch);
                break;
            case Drop::DropType::Doll:
                m_pPlayer->AddToPLayerScore(200);
                break;
            case Drop::DropType::MoneyBag:
                m_pPlayer->AddToPLayerScore(500);
                break;
            }
            drop->Kill();
        }
    }

    if (m_pPlayer->DoesWantToThrow())
    {
        SpawnPlayerWeapon(m_pPlayer->GetThrowPosition(),
            m_pPlayer->IsFacingRight(),
            m_pPlayer->GetPlayerWeapon());
    }
    

    for (Enemy* enemy : m_pEnemies)
    {
        if (enemy->IsDead()) { continue; }
        for (Projectile* proj : m_pPlayerProjectiles)
        {
            if (proj->IsDead()) { continue; }
            if (enemy->IsDead()) { continue; }

            if (utils::IsOverlapping(enemy->GetHitbox(), proj->GetHitbox()))
            {
                enemy->TakeDamage();

                if (enemy->IsDead())
                {
                    m_pPlayer->AddToPLayerScore(enemy->GetScore());

                    if (enemy->HasBag())
                    {
                        AddDrop(enemy->GetCenterPosition(), GetRandomBagDrop());
                    }

                    SpawnEffect(enemy->GetCenterPosition(),enemy->GetEffectType(),enemy->IsFacingRight());
                    if (enemy->GetEffectType() == Effect::EffectType::Fire)
                    {
                        m_pSoundManager->PlayEffect(SoundManager::SFX::FireDead);
                    }
                }
                else if (enemy->IsBoss())
                {
                    SpawnEffect(proj->GetCenterPosition(), Effect::EffectType::Blink, false);
                }

                proj->Kill();
            }
        }
    }

    RemoveDeadEntities();
    KillProjectilesOutsideSpawnArea();
}

void EntityManager::Draw(bool isDebug) const
{
    for (const Effect* effect : m_pEffects)
    {
        effect->Draw();
    }

    for (const Enemy* enemy : m_pEnemies)
    {
        enemy->Draw(); 
        if (enemy->HasBag())
        {
            enemy->DrawBag();
        }
        DrawGreenRectIfDebug(enemy->GetHitbox(), isDebug);
    }

    for (const Projectile* proj : m_pPlayerProjectiles)
    {

        proj->Draw(); 
        DrawGreenRectIfDebug(proj->GetHitbox(), isDebug);

    }

    for (const Projectile* proj : m_pEnemyProjectiles)
    {
        proj->Draw();
        DrawGreenRectIfDebug(proj->GetHitbox(), isDebug);

    }
    for (const Drop* drop : m_pDrops)
    {
        drop->Draw();
        DrawGreenRectIfDebug(drop->GetHitbox(), isDebug);

    }

    DrawGreenRectIfDebug(m_pPlayer->GetHitbox(), isDebug);


    if (isDebug)
    {
        DebugSpawnDraw();
    }
}

void EntityManager::DebugSpawnDraw() const
{
    utils::SetColor(Color4f{ 0, 1, 0, 1 });

   const std::vector<Level::EnemySpawnPoint>& spawnPoints = m_pLevel->GetEnemySpawnPoints();

    for (const Level::EnemySpawnPoint& spawnPoint : spawnPoints)
    {
        utils::DrawEllipse(spawnPoint.position, 10, 10);
    }
    const std::vector<Level::EnemySpawnArea>& spawnAreas = m_pLevel->GetEnemySpawnAreas();

    for (const  Level::EnemySpawnArea& spawnArea : spawnAreas)
    {
        utils::DrawRect(spawnArea.area);
    }
    Vector2f playerPos = m_pPlayer->GetCenterPosition();

    utils::DrawEllipse(playerPos, m_XSpawnLenth, m_XSpawnLenth);

    const std::vector<Level::DropSpawnPoint>& dropspawnPoints = m_pLevel->GetDropSpawnPoints();

    for (const  Level::DropSpawnPoint& spawnPoint : dropspawnPoints)
    {
        utils::DrawEllipse(spawnPoint.position, 10, 10);
    }
}
void EntityManager::SpawnEnemyByType(Level::EnemyType type, const Vector2f& pos, bool startsFacingRight)
{
    switch (type)
    {
    case Level::EnemyType::Zombie:
        AddZombie(pos, startsFacingRight);
        break;
    case Level::EnemyType::Bird:
        AddBird(pos, startsFacingRight);
        break;
    case Level::EnemyType::FlyingKnight:
        AddFlyingKnight(pos, startsFacingRight);
        break;
    case Level::EnemyType::Ghost:
        AddGhost(pos, startsFacingRight);
        break;
    case Level::EnemyType::Plant:
        AddPlant(pos);
        break;
    case Level::EnemyType::Demon:
        AddDemon(pos);
        break;
    case Level::EnemyType::Troll:
        AddTroll(pos);
        break;
    }
}
void EntityManager::SpawnPointEnemies()
{
    if (m_pLevel == nullptr || m_pPlayer == nullptr)
    {
        return;
    }

    const std::vector<Level::EnemySpawnPoint>& spawnPoints = m_pLevel->GetEnemySpawnPoints();
    Vector2f playerPos = m_pPlayer->GetCenterPosition();

    for (size_t i = 0; i < spawnPoints.size(); ++i)
    {
        if (spawnPoints[i].spawned)
        {
            continue;
        }
        float dx = std::abs(spawnPoints[i].position.x - playerPos.x);
        if (dx <= m_UpdateLenth)
        {
            bool faceRight = playerPos.x > spawnPoints[i].position.x;
            SpawnEnemyByType(spawnPoints[i].type, spawnPoints[i].position, faceRight);
            m_pLevel->markEnemySpawnPointSpawned(i);
        }
    }
}
void EntityManager::SpawnPointDrops()
{
    if (m_pLevel == nullptr || m_pPlayer == nullptr)
    {
        return;
    }

    const std::vector<Level::DropSpawnPoint>& dropSpawnPoints = m_pLevel->GetDropSpawnPoints();
    Vector2f playerPos = m_pPlayer->GetCenterPosition();

    for (size_t i = 0; i < dropSpawnPoints.size(); ++i)
    {
        if (dropSpawnPoints[i].spawned)
        {
            continue;
        }

        float dx = std::abs(dropSpawnPoints[i].position.x - playerPos.x);

        if (dx <= m_UpdateLenth)
        {
            AddDrop(dropSpawnPoints[i].position, dropSpawnPoints[i].type);
            m_pLevel->markDropSpawnPointSpawned(i);

        }
    }
}

void EntityManager::SpawnAreaEnemies(float elapsedSec)
{
    if (m_pLevel == nullptr || m_pPlayer == nullptr)
    {
        return;
    }

    const std::vector<Level::EnemySpawnArea>& spawnAreas = m_pLevel->GetEnemySpawnAreas();
    Vector2f playerPos = m_pPlayer->GetCenterPosition();

    for (size_t i = 0; i < spawnAreas.size(); ++i)
    {
        if (!(utils::IsPointInRect(playerPos, spawnAreas[i].area) && m_pLevel->IsEnemyAreaReadyToSpawn(i)))
        {
            continue;
        }
        Rectf targetSpawnArea{};
        targetSpawnArea.left = playerPos.x - m_XSpawnLenth;
        targetSpawnArea.width = m_XSpawnLenth * 2.f;
        targetSpawnArea.bottom = m_YMinSpawnForAir;
        targetSpawnArea.height = m_YMaxHeight;

        float areaLeft = spawnAreas[i].area.left;
        float areaRight = spawnAreas[i].area.left + spawnAreas[i].area.width;

        if (targetSpawnArea.left < areaLeft)
        {
            targetSpawnArea.left = areaLeft;
        }

        if (targetSpawnArea.left + targetSpawnArea.width > areaRight)
        {
            targetSpawnArea.width = areaRight - targetSpawnArea.left;
        }

        if (targetSpawnArea.width <= 0.f)
        {
            continue;
        }

        float x = targetSpawnArea.left +
            static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * targetSpawnArea.width;
        float y{ 0.f };

        if (!spawnAreas[i].SpawnAtTheGround)
        {
            float groundY{ 0.f };
            Vector2f rayStart{ x, playerPos.y + m_YMaxHeight };

            if (!FindGroundBelow(rayStart, groundY))
            {
                continue;
            }

            float minY = groundY + 30.f;
            float maxY = targetSpawnArea.bottom + targetSpawnArea.height;

            if (minY > maxY)
            {
                continue;
            }

            y = minY + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (maxY - minY);
        }
        else
        {
            Vector2f rayStart{ x, playerPos.y };

            if (!FindGroundBelow(rayStart, y))
            {
                continue;
            }
        }

        bool faceRight = playerPos.x > x;
        SpawnEnemyByType(spawnAreas[i].type, Vector2f{ x, y }, faceRight);
    }
}
bool EntityManager::FindGroundBelow(const Vector2f& pos, float& outGroundY) const
{
    if (m_pLevel == nullptr)
    {
        return false;
    }

    utils::HitInfo hitInfo{};

    bool hitGround = utils::LoopOverVertices(
        m_pLevel->GetVertices(),
        pos,
        Vector2f{ pos.x, pos.y - 1000.f },
        hitInfo);

    if (!hitGround)
    {
        return false;
    }

    outGroundY = hitInfo.intersectPoint.y;
    return true;
}

void EntityManager::AddZombie(const Vector2f& spawnPos, bool startsFacingRight)
{
    if (m_pLevel != nullptr)
    {
        Zombie* zombie = new Zombie(spawnPos, startsFacingRight, &m_pLevel->GetVertices());
        zombie->SetBag(RollBagDrop());
        m_pEnemies.push_back(zombie);
    }
}
void EntityManager::AddBird(const Vector2f& spawnPos, bool startsFacingRight)
{
    Bird* bird = new Bird(spawnPos, startsFacingRight);

    m_pEnemies.push_back(bird);
}
void EntityManager::AddFlyingKnight(const Vector2f& spawnPos, bool startsFacingRight)
{
    FlyingKnight* knight = new FlyingKnight(spawnPos, startsFacingRight);

    m_pEnemies.push_back(knight);
}
void EntityManager::AddGhost(const Vector2f& SpawnPos, bool startsFacingRight)
{
    Ghost* ghost = new Ghost(SpawnPos, startsFacingRight);
    ghost->SetEntityManager(this);
    ghost->SetBag(RollBagDrop());
    m_pEnemies.push_back(ghost);
}
void EntityManager::AddPlant(const Vector2f& spawnPos)
{
    Plant* plant = new Plant(spawnPos, this);
    m_pEnemies.push_back(plant);
}
void EntityManager::AddDemon(const Vector2f& spawnPos)
{
    Demon* demon = new Demon(spawnPos, this);
    m_pEnemies.push_back(demon);
}

void EntityManager::AddTroll(const Vector2f& spawnPos)
{
    if (m_pLevel != nullptr)
    {
        Troll* troll = new Troll(spawnPos, &m_pLevel->GetVertices(), this);

        m_pEnemies.push_back(troll);
    }
}
void EntityManager::SpawnPlayerWeapon(const Vector2f& pos, bool isRight,
    Player::PlayerWeapon weapon)
{
    m_pSoundManager->PlayEffect(SoundManager::SFX::Throw);
    const Vector2f direction{ isRight ? 1.f : -1.f, 0.f };

    switch (weapon)
    {
    case Player::PlayerWeapon::Lance:
        m_pPlayerProjectiles.push_back(
            new SimpleProjectile{ pos, direction, SimpleProjectile::SimpleProjectileType::Lance });
        break;
    case Player::PlayerWeapon::Knife:
        m_pPlayerProjectiles.push_back(
            new SimpleProjectile{ pos, direction, SimpleProjectile::SimpleProjectileType::Knife });
        break;
    case Player::PlayerWeapon::Torch:
        SpawnTorch(pos, isRight);
        break;
    }
}

void EntityManager::SpawnEnemyProjectile(const Vector2f& pos, const Vector2f& direction,
    SimpleProjectile::SimpleProjectileType type)
{
    m_pEnemyProjectiles.push_back(new SimpleProjectile{ pos, direction, type });
}

void EntityManager::SpawnTorch(const Vector2f& pos, bool isRight)
{
    if (m_pLevel != nullptr)
    {
        Torch* torch = new Torch(pos, isRight, &m_pLevel->GetVertices());
        m_pPlayerProjectiles.push_back(torch);

    }
}

void EntityManager::AddDrop(const Vector2f& pos, Drop::DropType type)
{
    if (m_pLevel != nullptr)
    {
        Drop* drop = new Drop(pos, type, &m_pLevel->GetVertices());
        m_pDrops.push_back(drop);
    }
}
void EntityManager::SpawnEffect(const Vector2f& pos, Effect::EffectType type, bool isMirrored)
{
    Effect* effect = new Effect(pos, type, isMirrored);
    m_pEffects.push_back(effect);
}
void EntityManager::RemoveDeadEntities()
{
    for (size_t i = 0; i < m_pEnemies.size(); ++i)
    {
        if (m_pEnemies[i]->IsDead())
        {
            delete m_pEnemies[i];
            m_pEnemies.erase(m_pEnemies.begin() + i);
            --i;
        }
    }

    for (size_t i = 0; i < m_pPlayerProjectiles.size(); ++i)
    {
        if (m_pPlayerProjectiles[i]->IsDead())
        {
            delete m_pPlayerProjectiles[i];
            m_pPlayerProjectiles.erase(m_pPlayerProjectiles.begin() + i);
            --i;
        }
    }

    for (size_t i = 0; i < m_pEnemyProjectiles.size(); ++i)
    {
        if (m_pEnemyProjectiles[i]->IsDead())
        {
            delete m_pEnemyProjectiles[i];
            m_pEnemyProjectiles.erase(m_pEnemyProjectiles.begin() + i);
            --i;
        }
    }
    for (size_t i = 0; i < m_pDrops.size(); ++i)
    {
        if (m_pDrops[i]->IsDead())
        {
            delete m_pDrops[i];
            m_pDrops.erase(m_pDrops.begin() + i);
            --i;
        }
    }

    for (size_t i = 0; i < m_pEffects.size(); ++i)
    {
        if (m_pEffects[i]->IsFinished())
        {
            delete m_pEffects[i];
            m_pEffects.erase(m_pEffects.begin() + i);
            --i;
        }
    }
}
void EntityManager::SetLevel(Level* pLevel)
{
    m_pLevel = pLevel;
}


void EntityManager::SetPlayer(Player* pPlayer)
{
    m_pPlayer = pPlayer;
}

void EntityManager::SetSoundManager(SoundManager* pSoundManager)
{
    m_pSoundManager = pSoundManager;
}

Vector2f EntityManager::GetPlayerPosition() const
{
    if (m_pPlayer != nullptr)
    {
        return m_pPlayer->GetCenterPosition();
    }

    return Vector2f{ 0.f, 0.f };
}


void EntityManager::KillProjectilesOutsideSpawnArea()
{
    Vector2f playerPos = m_pPlayer->GetCenterPosition();

    Rectf activeArea
    {
        playerPos.x - m_UpdateLenth,
        playerPos.y - m_UpdateLenth,
        m_UpdateLenth * 2.f,
        m_UpdateLenth * 2.f
    };

    for (Projectile* proj : m_pPlayerProjectiles)
    {
        if (!utils::IsPointInRect(proj->GetCenterPosition(), activeArea))
        {
            proj->Kill();
        }
    }

    for (Projectile* proj : m_pEnemyProjectiles)
    {
        if (!utils::IsPointInRect(proj->GetCenterPosition(), activeArea))
        {
            proj->Kill();
        }
    }
}