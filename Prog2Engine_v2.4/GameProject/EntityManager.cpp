#include "pch.h"
#include "EntityManager.h"
#include "utils.h"

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

void EntityManager::Update(float elapsedSec, Player& player)
{
    for (Enemy* enemy : m_Enemies)
    {
        if (Ghost* ghost = dynamic_cast<Ghost*>(enemy))
        {
            ghost->Update(elapsedSec, player.GetCenterPosition());
        }
        else if (Plant* plant = dynamic_cast<Plant*>(enemy))
        {
            plant->Update(elapsedSec, player.GetCenterPosition());
        }
        else if (Demon* demon = dynamic_cast<Demon*>(enemy))
        {
            demon->Update(elapsedSec, player.GetCenterPosition());
        }
        else if (Troll* troll = dynamic_cast<Troll*>(enemy))
        {
            troll->Update(elapsedSec, player.GetCenterPosition());
        }
        else
        {
            enemy->Update(elapsedSec);
        }

        if (utils::IsOverlapping(enemy->GetHitbox(), player.GetHitbox()))
        {
            player.TakeDamage();
        }
    }


    for (Projectile* proj : m_PlayerProjectiles)
    {
        proj->Update(elapsedSec);
    }
    for (Projectile* proj : m_EnemyProjectiles)
    {
        proj->Update(elapsedSec);
        if (utils::IsOverlapping(player.GetHitbox(), proj->GetHitbox()))
        {
            player.TakeDamage();
            proj->Kill();
        }
    }
    for (Drop* drop : m_Drops)
    {
        drop->Update(elapsedSec);
        if (utils::IsOverlapping(player.GetHitbox(), drop->GetHitbox()))
        {
            switch (drop->GetType())
            {
                case PickupType::Lance:
                    player.SetPlayerWeapon(PlayerWeapon::Lance);
                break;
                case PickupType::Knife:
                    player.SetPlayerWeapon(PlayerWeapon::Knife);
                break;
                case PickupType::Torch:
                    player.SetPlayerWeapon(PlayerWeapon::Tourch);
                break;
                case PickupType::Doll:

                break;
                case PickupType::MoneyBag:

                break;
            }
            drop->Kill();
        }
    }

    if (player.DoesWantToThrow())
    {
        switch (player.GetPlayerWeapon())
        {
        case PlayerWeapon::Lance:
            SpawnLance(player.GetCenterPosition(), player.IsFacingRight());
            break;
        case PlayerWeapon::Knife:
            SpawnKnife(player.GetCenterPosition(), player.IsFacingRight());
            break;
        case PlayerWeapon::Tourch:
            SpawnTourch(player.GetCenterPosition(), player.IsFacingRight());
            break;
        }

    }

    for (Enemy* zomb : m_Enemies)
    {
        for (Projectile* proj : m_PlayerProjectiles)
        {
            if (utils::IsOverlapping(zomb->GetHitbox(), proj->GetHitbox()))
            {
                zomb->TakeDamage();
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
void EntityManager::SetLevel(const Level* pLevel)
{
    m_pLevel = pLevel;
}

