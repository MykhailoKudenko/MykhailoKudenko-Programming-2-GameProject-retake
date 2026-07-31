#include "pch.h"
#include "EntityManager.h"
#include "utils.h"
#include <cstdlib>
#include <cmath>

EntityManager::EntityManager()
{
    //test
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
                enemy->SetIsActive(false);
        }

        if (enemy->GetIsActive())
        {
            if (dynamic_cast<Ghost*>(enemy) != nullptr)
            {
                hasGhostInRange = true;
            }

            if (dynamic_cast<FlyingKnight*>(enemy) != nullptr)
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
            m_pSoundManager->PlayEffect(SFX::Ghost);
        }
        m_GhostSoundTimer = m_GhostSoundCooldown;
    }

    if (hasFlyingKnightInRange && m_FlyingKnightSoundTimer <= 0.f)
    {
        if (m_pSoundManager != nullptr)
        {
            m_pSoundManager->PlayEffect(SFX::FlyingKnight);
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
            m_pSoundManager->PlayEffect(SFX::PickUp);
            switch (drop->GetType())
            {
            case PickupType::Lance:
                m_pPlayer->SetPlayerWeapon(PlayerWeapon::Lance);
                break;
            case PickupType::Knife:
                m_pPlayer->SetPlayerWeapon(PlayerWeapon::Knife);
                break;
            case PickupType::Torch:
                m_pPlayer->SetPlayerWeapon(PlayerWeapon::Torch);
                break;
            case PickupType::Doll:
                m_pPlayer->AddToPLayerScore(200);
                break;
            case PickupType::MoneyBag:
                m_pPlayer->AddToPLayerScore(500);
                break;
            }
            drop->Kill();
        }
    }

    if (m_pPlayer->DoesWantToThrow())
    {
        switch (m_pPlayer->GetPlayerWeapon())
        {
        case PlayerWeapon::Lance:
            SpawnLance(m_pPlayer->GetThrowPosition(), m_pPlayer->IsFacingRight());
            break;
        case PlayerWeapon::Knife:
            SpawnKnife(m_pPlayer->GetThrowPosition(), m_pPlayer->IsFacingRight());
            break;
        case PlayerWeapon::Torch:
            SpawnTorch(m_pPlayer->GetThrowPosition(), m_pPlayer->IsFacingRight());
            break;
        }
    }

    for (Enemy* enemy : m_pEnemies)
    {
        for (Projectile* proj : m_pPlayerProjectiles)
        {
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
                        m_pSoundManager->PlayEffect(SFX::FireDead);
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

        if (isDebug)
        {
            utils::SetColor(Color4f{ 0, 1, 0, 1 });
            utils::DrawRect(enemy->GetHitbox());
        }
    }

    for (const Projectile* proj : m_pPlayerProjectiles)
    {

        proj->Draw(); 
        if (isDebug)
        {
            utils::SetColor(Color4f{ 0, 1, 0, 1 });
            utils::DrawRect(proj->GetHitbox());
        }
    }

    for (const Projectile* proj : m_pEnemyProjectiles)
    {
        proj->Draw();
        if (isDebug)
        {
            utils::SetColor(Color4f{ 0, 1, 0, 1 });
            utils::DrawRect(proj->GetHitbox());
        }
    }
    for (const Drop* drop : m_pDrops)
    {
        drop->Draw();
        if (isDebug)
        {
            utils::SetColor(Color4f{ 0, 1, 0, 1 });
            utils::DrawRect(drop->GetHitbox());
        }
    }

    if (isDebug)
    {
        utils::SetColor(Color4f{ 0, 1, 0, 1 });
        utils::DrawRect(m_pPlayer->GetHitbox());

        DebugSpawnDraw();
    }
}

void EntityManager::DebugSpawnDraw() const
{
    utils::SetColor(Color4f{ 0, 1, 0, 1 });

   std::vector<Level::EnemySpawnPoint>& spawnPoints = m_pLevel->GetEnemySpawnPoints();

    for (Level::EnemySpawnPoint& spawnPoint : spawnPoints)
    {
        utils::DrawEllipse(spawnPoint.position, 10, 10);
    }
     std::vector<Level::EnemySpawnArea>& spawnAreas = m_pLevel->GetEnemySpawnAreas();

    for (  Level::EnemySpawnArea& spawnArea : spawnAreas)
    {
        utils::DrawRect(spawnArea.area);
    }
    Vector2f playerPos = m_pPlayer->GetCenterPosition();

    utils::DrawEllipse(playerPos, m_XSpawnLenth, m_XSpawnLenth);

     std::vector<Level::DropSpawnPoint>& DropspawnPoints = m_pLevel->GetDropSpawnPoints();

    for (  Level::DropSpawnPoint& SpawnPoint : DropspawnPoints)
    {
        utils::DrawEllipse(SpawnPoint.position, 10, 10);
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
    if (m_pLevel == nullptr)
    {
        return;
    }

    std::vector<Level::EnemySpawnPoint>& spawnPoints = m_pLevel->GetEnemySpawnPoints();

    for (Level::EnemySpawnPoint& spawnPoint : spawnPoints)
    {
        if (spawnPoint.spawned)
        {
            continue;
        }
        bool faceRight = m_pPlayer->GetCenterPosition().x > spawnPoint.position.x;
        SpawnEnemyByType(spawnPoint.type, spawnPoint.position, faceRight);
        spawnPoint.spawned = true;
    }
}
void EntityManager::SpawnPointDrops()
{
    if (m_pLevel == nullptr || m_pPlayer == nullptr)
    {
        return;
    }

    std::vector<Level::DropSpawnPoint>& dropSpawnPoints = m_pLevel->GetDropSpawnPoints();
    Vector2f playerPos = m_pPlayer->GetCenterPosition();

    for ( Level::DropSpawnPoint& dropSpawn : dropSpawnPoints)
    {
        if (dropSpawn.spawned)
        {
            continue;
        }

        float dx = std::abs(dropSpawn.position.x - playerPos.x);

        if (dx <= m_UpdateLenth)
        {
            AddDrop(dropSpawn.position, dropSpawn.type);
            dropSpawn.spawned = true;
        }
    }
}

void EntityManager::SpawnAreaEnemies(float elapsedSec)
{
    if (m_pLevel == nullptr || m_pPlayer == nullptr)
    {
        return;
    }

    std::vector<Level::EnemySpawnArea>& spawnAreas = m_pLevel->GetEnemySpawnAreas();
    Vector2f playerPos = m_pPlayer->GetCenterPosition();

    for (Level::EnemySpawnArea& spawnArea : spawnAreas)
    {
        if (!utils::IsPointInRect(playerPos, spawnArea.area))
        {
            spawnArea.timer = 0.f;
            continue;
        }

        spawnArea.timer += elapsedSec;

        if (spawnArea.timer < spawnArea.timerMax)
        {
            continue;
        }

        spawnArea.timer = 0.f;

        Rectf targetSpawnArea{};
        targetSpawnArea.left = playerPos.x - m_XSpawnLenth;
        targetSpawnArea.width = m_XSpawnLenth * 2.f;
        targetSpawnArea.bottom = m_YMinSpawnForAir;
        targetSpawnArea.height = m_YMaxHeight;

        float areaLeft = spawnArea.area.left;
        float areaRight = spawnArea.area.left + spawnArea.area.width;

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
            float(std::rand()) / float(RAND_MAX) * targetSpawnArea.width;

        float y{ 0.f };

        if (!spawnArea.SpawnAtTheGround)
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

            y = minY + float(std::rand()) / float(RAND_MAX) * (maxY - minY);
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
        SpawnEnemyByType(spawnArea.type, Vector2f{ x, y }, faceRight);
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
    Zombie* zombie = new Zombie(spawnPos, startsFacingRight);

    if (m_pLevel != nullptr)
    {
        zombie->SetWorld(&m_pLevel->GetVertices());
    }

    zombie->SetBag(RollBagDrop());

    m_pEnemies.push_back(zombie);
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
    Plant* plant = new Plant(spawnPos);
    plant->SetEntityManager(this);
    m_pEnemies.push_back(plant);
}
void EntityManager::AddDemon(const Vector2f& spawnPos)
{
    Demon* demon = new Demon(spawnPos);
    demon->SetEntityManager(this);
    m_pEnemies.push_back(demon);
}

void EntityManager::AddTroll(const Vector2f& spawnPos)
{
    Troll* troll = new Troll(spawnPos);
    troll->SetEntityManager(this);
    if (m_pLevel != nullptr)
    {
        troll->SetWorld(&m_pLevel->GetVertices());
    }
    m_pEnemies.push_back(troll);
}
void EntityManager::SpawnLance(const Vector2f& pos, bool isRight)
{
    m_pSoundManager->PlayEffect(SFX::Throw);
    Lance* lance = new Lance(pos, isRight);
    m_pPlayerProjectiles.push_back(lance);
}
void EntityManager::SpawnKnife(const Vector2f& pos, bool isRight)
{
    m_pSoundManager->PlayEffect(SFX::Throw);
    Knife* knife = new Knife(pos, isRight);
    m_pPlayerProjectiles.push_back(knife);

}
void EntityManager::SpawnTorch(const Vector2f& pos, bool isRight)
{
    m_pSoundManager->PlayEffect(SFX::Throw);
    Torch* torch = new Torch(pos, isRight);
    if (m_pLevel != nullptr)
    {
        torch->SetWorld(&m_pLevel->GetVertices());
    }
    m_pPlayerProjectiles.push_back(torch);
}

void EntityManager::SpawnPlantProjectile(const Vector2f& pos, const Vector2f& direction)
{
    PlantProjectile* proj = new PlantProjectile(pos, direction);

    m_pEnemyProjectiles.push_back(proj);
}

void EntityManager::SpawnDemonProjectile(const Vector2f& pos, const Vector2f& direction)
{
    DemonProjectile* proj = new DemonProjectile(pos, direction);

    m_pEnemyProjectiles.push_back(proj);
}
void EntityManager::AddDrop(const Vector2f& pos, PickupType type)
{
    Drop* drop = new Drop(pos, type);
    if (m_pLevel != nullptr)
    {
        drop->SetWorld(&m_pLevel->GetVertices());
    }
    m_pDrops.push_back(drop);
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

bool EntityManager::RollBagDrop() const
{
    return (std::rand() % 10) == 0;
}

PickupType EntityManager::GetRandomBagDrop() const
{
    int randomIndex = std::rand() % 5;

    switch (randomIndex)
    {
    case 0: return PickupType::Lance;
    case 1: return PickupType::Knife;
    case 2: return PickupType::Torch;
    case 3: return PickupType::Doll;
    case 4: return PickupType::MoneyBag;
    default: return PickupType::Doll;
    }
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