#include "pch.h"
#include "EntityManager.h"
#include "utils.h"
#include <cstdlib>
#include <cmath>

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
    for (Enemy* enemy : m_Enemies)
    {
        delete enemy;
    }
    for (Projectile* proj : m_PlayerProjectiles)
    {
        delete proj;
    }
    for (Projectile* proj : m_EnemyProjectiles)
    {
        delete proj;
    }
    for (Drop* drop : m_Drops)
    {
        delete drop;
    }
}

void EntityManager::Update(float elapsedSec)
{
    if (m_pPlayer == nullptr || m_pLevel == nullptr)
    {
        return;
    }

    SpawnPointEnemies();
    SpawnAreaEnemies(elapsedSec);

    Vector2f playerPos = m_pPlayer->GetCenterPosition();

    for (Enemy* enemy : m_Enemies)
    {
        float dx = std::abs(enemy->GetCenterPosition().x - playerPos.x);

        if (!enemy->GetIsActive() && dx <= UpdateLenth)
        {
            enemy->SetIsActive(true);
        }

        if (enemy->GetIsActive())
        {
            enemy->Update(elapsedSec);

            if (utils::IsOverlapping(enemy->GetHitbox(), m_pPlayer->GetHitbox()))
            {
                if (!enemy->isSpawning())
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

    for (Projectile* proj : m_PlayerProjectiles)
    {
        proj->Update(elapsedSec);
    }

    for (Projectile* proj : m_EnemyProjectiles)
    {
        proj->Update(elapsedSec);
        if (utils::IsOverlapping(m_pPlayer->GetHitbox(), proj->GetHitbox()))
        {
            m_pPlayer->TakeDamage();
            proj->Kill();
        }
    }

    for (Drop* drop : m_Drops)
    {
        drop->Update(elapsedSec);
        if (utils::IsOverlapping(m_pPlayer->GetHitbox(), drop->GetHitbox()))
        {
            switch (drop->GetType())
            {
            case PickupType::Lance:
                m_pPlayer->SetPlayerWeapon(PlayerWeapon::Lance);
                break;
            case PickupType::Knife:
                m_pPlayer->SetPlayerWeapon(PlayerWeapon::Knife);
                break;
            case PickupType::Torch:
                m_pPlayer->SetPlayerWeapon(PlayerWeapon::Tourch);
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
            SpawnLance(m_pPlayer->GetCenterPosition(), m_pPlayer->IsFacingRight());
            break;
        case PlayerWeapon::Knife:
            SpawnKnife(m_pPlayer->GetCenterPosition(), m_pPlayer->IsFacingRight());
            break;
        case PlayerWeapon::Tourch:
            SpawnTourch(m_pPlayer->GetCenterPosition(), m_pPlayer->IsFacingRight());
            break;
        }
    }

    for (Enemy* enemy : m_Enemies)
    {
        for (Projectile* proj : m_PlayerProjectiles)
        {
            if (utils::IsOverlapping(enemy->GetHitbox(), proj->GetHitbox()))
            {
                enemy->TakeDamage();
                if (enemy->isDead())
                {
                    m_pPlayer->AddToPLayerScore(enemy->GetScore());
                }
                proj->Kill();
            }
        }
    }

    RemoveDeadEntities();
}



void EntityManager::Draw() const
{
    for (const Enemy* enemy : m_Enemies)
    {
        enemy->Draw();
        enemy->DrawCollider();
    }

    for (const Projectile* proj : m_PlayerProjectiles)
    {

        proj->Draw();
        proj->DrawCollider();
    }

    for (const Projectile* proj : m_EnemyProjectiles)
    {
        proj->Draw();
        proj->DrawCollider();
    }
    for (const Drop* drop : m_Drops)
    {
        drop->Draw();
        utils::SetColor(Color4f{ 0, 1, 0, 1 });
        utils::DrawRect(drop->GetHitbox());
    }

    utils::SetColor(Color4f{ 0, 1, 0, 1 });
    utils::DrawRect(m_pPlayer->GetHitbox());

    DebugSpawnDraw();
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

    for (Level::EnemySpawnArea& spawnArea : spawnAreas)
    {
        utils::DrawRect(spawnArea.area);
    }
    Vector2f playerPos = m_pPlayer->GetCenterPosition();

    utils::DrawEllipse(playerPos, xSpawnLenth, xSpawnLenth);

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

        if (spawnArea.timer < m_AreaSpawnInterval)
        {
            continue;
        }

        spawnArea.timer = 0.f;

        Rectf targetSpawnArea{};
        targetSpawnArea.left = playerPos.x - xSpawnLenth;
        targetSpawnArea.width = xSpawnLenth * 2.f;
        targetSpawnArea.bottom = yMinSpawnForAir;
        targetSpawnArea.height = yMaxHeight;

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
            y = targetSpawnArea.bottom +
                float(std::rand()) / float(RAND_MAX) * targetSpawnArea.height;
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

    bool hitGround = utils::LoopOverVertecies(
        m_pLevel->GetVertecies(),
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

void EntityManager::AddZombie(const Vector2f& SpawnPos, bool startsFacingRight)
{
    Zombie* zombie = new Zombie(SpawnPos, startsFacingRight);

    if (m_pLevel != nullptr)
    {
        zombie->SetWorld(&m_pLevel->GetVertecies());
    }

    m_Enemies.push_back(zombie);
}
void EntityManager::AddBird(const Vector2f& SpawnPos, bool startsFacingRight)
{
    Bird* bird = new Bird(SpawnPos, startsFacingRight);

    m_Enemies.push_back(bird);
}
void EntityManager::AddFlyingKnight(const Vector2f& SpawnPos, bool startsFacingRight)
{
    FlyingKnight* knight = new FlyingKnight(SpawnPos, startsFacingRight);

    m_Enemies.push_back(knight);
}
void EntityManager::AddGhost(const Vector2f& SpawnPos, bool startsFacingRight)
{
    Ghost* ghost = new Ghost(SpawnPos, startsFacingRight);
    ghost->SetEntityManager(this);
    m_Enemies.push_back(ghost);
}
void EntityManager::AddPlant(const Vector2f& spawnPos)
{
    Plant* palnt = new Plant(spawnPos);
    palnt->SetEntityManager(this);
    m_Enemies.push_back(palnt);
}
void EntityManager::AddDemon(const Vector2f& spawnPos)
{
    Demon* demon = new Demon(spawnPos);
    demon->SetEntityManager(this);
    m_Enemies.push_back(demon);
}

void EntityManager::AddTroll(const Vector2f& spawnPos)
{
    Troll* troll = new Troll(spawnPos);
    troll->SetEntityManager(this);
    if (m_pLevel != nullptr)
    {
        troll->SetWorld(&m_pLevel->GetVertecies());
    }
    m_Enemies.push_back(troll);
}
void EntityManager::SpawnLance(const Vector2f& pos, bool isRight)
{
    Lance* lance = new Lance(pos, isRight);
    m_PlayerProjectiles.push_back(lance);
}
void EntityManager::SpawnKnife(const Vector2f& pos, bool isRight)
{
    Knife* knife = new Knife(pos, isRight);
    m_PlayerProjectiles.push_back(knife);

}
void EntityManager::SpawnTourch(const Vector2f& pos, bool isRight)
{
    Torch* torch = new Torch(pos, isRight);
    if (m_pLevel != nullptr)
    {
        torch->SetWorld(&m_pLevel->GetVertecies());
    }
    m_PlayerProjectiles.push_back(torch);
}

void EntityManager::SpawnPlantProjectile(const Vector2f& pos, const Vector2f& direction)
{
    PlantProjectile* proj = new PlantProjectile(pos, direction);

    m_EnemyProjectiles.push_back(proj);
}

void EntityManager::SpawnDemonProjectile(const Vector2f& pos, const Vector2f& direction)
{
    DemonProjectile* proj = new DemonProjectile(pos, direction);

    m_EnemyProjectiles.push_back(proj);
}
void EntityManager::AddDrop(const Vector2f& pos, PickupType type)
{
    Drop* drop = new Drop(pos, type);
    if (m_pLevel != nullptr)
    {
        drop->SetWorld(&m_pLevel->GetVertecies());
    }
    m_Drops.push_back(drop);
}

void EntityManager::RemoveDeadEntities()
{
    for (int i = 0; i < m_Enemies.size(); ++i)
    {
        if (m_Enemies[i]->isDead())
        {
            delete m_Enemies[i];
            m_Enemies.erase(m_Enemies.begin() + i);
            --i;
        }
    }

    for (int i = 0; i < m_PlayerProjectiles.size(); ++i)
    {
        if (m_PlayerProjectiles[i]->isDead())
        {
            delete m_PlayerProjectiles[i];
            m_PlayerProjectiles.erase(m_PlayerProjectiles.begin() + i);
            --i;
        }
    }

    for (int i = 0; i < m_EnemyProjectiles.size(); ++i)
    {
        if (m_EnemyProjectiles[i]->isDead())
        {
            delete m_EnemyProjectiles[i];
            m_EnemyProjectiles.erase(m_EnemyProjectiles.begin() + i);
            --i;
        }
    }
    for (int i = 0; i < m_Drops.size(); ++i)
    {
        if (m_Drops[i]->IsDead())
        {
            delete m_Drops[i];
            m_Drops.erase(m_Drops.begin() + i);
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
Vector2f EntityManager::GetPlayerPosition() const
{
    if (m_pPlayer != nullptr)
    {
        return m_pPlayer->GetCenterPosition();
    }

    return Vector2f{ 0.f, 0.f };
}