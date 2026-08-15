#include "pch.h"
#include "EntityManager.h"
#include "utils.h"
#include <cstdlib>
#include <cmath>
#include <iostream>

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
    }
}

std::ostream& operator<<(std::ostream& out, const EntityManager& manager)
{
    out << "Enemy: " << manager.m_pEnemies.size()
    << " PlayerProj: " << manager.m_pPlayerProjectiles.size()
    << " EnemyProj: " << manager.m_pEnemyProjectiles.size()
    << " Drops: " << manager.m_Drops.size()
    << " Effects: " << manager.m_Effects.size();
    return out;
}

EntityManager::EntityManager():

m_pPlayer{ nullptr },
m_pLevel{ nullptr },

m_pEnemies{ },
m_pPlayerProjectiles{ },
m_pEnemyProjectiles{ },
m_Drops{ },
m_Effects{ },

m_UpdateLength{ 200.0 },

m_XSpawnLength{ 200.0 },
m_XKillLength{ 400.0 },

m_YMinSpawnForAir{ 37.f },
m_YMaxHeight{ 200.f },

m_GhostSoundTimer{ 0.f },
m_FlyingKnightSoundTimer{ 0.f },

m_GhostSoundCooldown{ 2.0f },
m_FlyingKnightSoundCooldown{ 2.0f }
{

}

EntityManager::~EntityManager()
{
    DeleteAllEntities();
}

void EntityManager::DeleteAllEntities()
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

    m_Drops.clear();

    m_Effects.clear();
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

    EnemiesUpdate(elapsedSec);
    EnemiesBulletCollsion();

    EffectUpdate(elapsedSec);
    ProjectileUpdate(elapsedSec);
    DropsUpdate(elapsedSec);

    RemoveDeadEntities();
    KillProjectilesOutsideSpawnArea();
}

void EntityManager::Draw(bool isDebug) const
{
    for (const Effect& effect : m_Effects)
    {
        effect.Draw();
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
    for (const Drop& drop : m_Drops)
    {
        drop.Draw();
        DrawGreenRectIfDebug(drop.GetHitbox(), isDebug);

    }

    DrawGreenRectIfDebug(m_pPlayer->GetHitbox(), isDebug);


    if (isDebug)
    {
        DebugSpawnDraw();
    }
}

void EntityManager::EnemiesUpdate(float elapsedSec)
{
    Vector2f playerPos = m_pPlayer->GetCenterPosition();
    m_GhostSoundTimer -= elapsedSec;
    m_FlyingKnightSoundTimer -= elapsedSec;

    bool hasGhostInRange{ false };
    bool hasFlyingKnightInRange{ false };
    for (Enemy* enemy : m_pEnemies)
    {
        float dx = std::abs(enemy->GetCenterPosition().x - playerPos.x);


        if (dx <= m_UpdateLength)
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
        if (enemy->GetCenterPosition().y < -20.f)
        {
            enemy->Kill();
            continue;
        }
        if (dx > m_XKillLength && enemy->IsBoss() == false)
        {
            enemy->Kill();
            continue;
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



    }

    if (hasGhostInRange && m_GhostSoundTimer <= 0.f)
    {

        SoundManager::GetInstance().PlayEffect(SoundManager::SFX::Ghost);

        m_GhostSoundTimer = m_GhostSoundCooldown;
    }

    if (hasFlyingKnightInRange && m_FlyingKnightSoundTimer <= 0.f)
    {

        SoundManager::GetInstance().PlayEffect(SoundManager::SFX::FlyingKnight);

        m_FlyingKnightSoundTimer = m_FlyingKnightSoundCooldown;
    }
}
void EntityManager::EnemiesBulletCollsion()
{
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
                    m_pPlayer->AddToPlayerScore(enemy->GetScore());

                    if (enemy->HasBag())
                    {
                        AddDrop(enemy->GetCenterPosition(), GetRandomBagDrop());
                    }

                    SpawnEffect(enemy->GetCenterPosition(), enemy->GetEffectType(), enemy->IsFacingRight());
                    if (enemy->GetEffectType() == Effect::EffectType::Fire)
                    {
                        SoundManager::GetInstance().PlayEffect(SoundManager::SFX::FireDead);
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
}
void EntityManager::DropsUpdate(float elapsedSec)
{
    Vector2f playerPos = m_pPlayer->GetCenterPosition();

    for (Drop& drop : m_Drops)
    {
        float dx = std::abs(drop.GetHitbox().left+ drop.GetHitbox().width/2 - playerPos.x);

        if (dx <= m_UpdateLength)
        {
            drop.SetIsActive(true);
        }
        else
        {
            drop.SetIsActive(false);
        }
        if (drop.GetHitbox().bottom < -20.f)
        {
            drop.Kill();
            continue;
        }
        if (dx > m_XKillLength)
        {
            drop.Kill();
            continue;
        }
        drop.Update(elapsedSec);


        if (utils::IsOverlapping(m_pPlayer->GetHitbox(), drop.GetHitbox()))
        {

            SoundManager::GetInstance().PlayEffect(SoundManager::SFX::PickUp);

            switch (drop.GetType())
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
                m_pPlayer->AddToPlayerScore(200);
                break;
            case Drop::DropType::MoneyBag:
                m_pPlayer->AddToPlayerScore(500);
                break;
            }
            drop.Kill();
        }
    }
}
void EntityManager::ProjectileUpdate(float elapsedSec)
{
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

}
void EntityManager::EffectUpdate(float elapsedSec)
{
    for (Effect& effect : m_Effects)
    {
        effect.Update(elapsedSec);
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

    utils::DrawEllipse(playerPos, m_XSpawnLength, m_XSpawnLength);

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
        if (dx <= m_UpdateLength)
        {
            bool faceRight = playerPos.x > spawnPoints[i].position.x;
            SpawnEnemyByType(spawnPoints[i].type, spawnPoints[i].position, faceRight);
            m_pLevel->MarkEnemySpawnPointSpawned(i);
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

        if (dx <= m_UpdateLength)
        {
            AddDrop(dropSpawnPoints[i].position, dropSpawnPoints[i].type);
            m_pLevel->MarkDropSpawnPointSpawned(i);

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
        targetSpawnArea.left = playerPos.x - m_XSpawnLength;
        targetSpawnArea.width = m_XSpawnLength * 2.f;
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

        if (!spawnAreas[i].spawnAtTheGround)
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
void EntityManager::AddGhost(const Vector2f& spawnPos, bool startsFacingRight)
{
    Ghost* ghost = new Ghost(spawnPos, startsFacingRight, this);
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
    SoundManager::GetInstance().PlayEffect(SoundManager::SFX::Throw);
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
        m_Drops.emplace_back(pos, type, &m_pLevel->GetVertices());

    }
}
void EntityManager::SpawnEffect(const Vector2f& pos, Effect::EffectType type, bool isMirrored)
{
    m_Effects.emplace_back(pos, type, isMirrored);
}
void EntityManager::RemoveDeadEntities()
{
    for (size_t i = 0; i < m_pEnemies.size(); ++i)
    {
        if (m_pEnemies[i]->IsDead())
        {
            delete m_pEnemies[i];
            m_pEnemies[i] = m_pEnemies.back();
            m_pEnemies.pop_back();
            --i;
        }
    }
    for (size_t i = 0; i < m_pPlayerProjectiles.size(); ++i)
    {
        if (m_pPlayerProjectiles[i]->IsDead())
        {
            delete m_pPlayerProjectiles[i];
            m_pPlayerProjectiles[i] = m_pPlayerProjectiles.back();
            m_pPlayerProjectiles.pop_back();
            --i;
        }
    }
    for (size_t i = 0; i < m_pEnemyProjectiles.size(); ++i)
    {
        if (m_pEnemyProjectiles[i]->IsDead())
        {
            delete m_pEnemyProjectiles[i];
            m_pEnemyProjectiles[i] = m_pEnemyProjectiles.back();
            m_pEnemyProjectiles.pop_back();
            --i;
        }
    }
    for (size_t i = 0; i < m_Drops.size(); ++i)
    {
        if (m_Drops[i].IsDead())
        {
            m_Drops[i] = m_Drops.back();
            m_Drops.pop_back();
            --i;
        }
    }
    for (size_t i = 0; i < m_Effects.size(); ++i)
    {
        if (m_Effects[i].IsFinished())
        {
            m_Effects[i] = m_Effects.back();
            m_Effects.pop_back();
            --i;
        }
    }
}
void EntityManager::SetLevel(Level* pLevel)
{
    m_pLevel = pLevel;
}
void EntityManager::SetPlayer(Player* pLevel)
{
    m_pPlayer = pLevel;
}
Vector2f EntityManager::GetPlayerPosition() const
{
    if (m_pPlayer != nullptr)
    {
        return m_pPlayer->GetCenterPosition();
    }

    return Vector2f{ 0.f, 0.f };
}

void EntityManager::Reset()
{
    DeleteAllEntities();
    m_pEnemies.clear();
    m_pPlayerProjectiles.clear();
    m_pEnemyProjectiles.clear();
    m_Drops.clear();
    m_Effects.clear();

    m_GhostSoundTimer = 0.f;
    m_FlyingKnightSoundTimer = 0.f;
}

void EntityManager::KillProjectilesOutsideSpawnArea()
{
    Vector2f playerPos = m_pPlayer->GetCenterPosition();

    Rectf activeArea
    {
        playerPos.x - m_UpdateLength,
        playerPos.y - m_UpdateLength,
        m_UpdateLength * 2.f,
        m_UpdateLength * 2.f
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