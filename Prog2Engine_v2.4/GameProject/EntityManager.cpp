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
}

void EntityManager::Update(float elapsedSec, Player& player)
{
    for (Enemy* enemy : m_Enemies)
    {
        Ghost* ghost = dynamic_cast<Ghost*>(enemy);

        if (ghost != nullptr)
        {
            ghost->Update(elapsedSec, player.GetCenterPosition());
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

    for (Projectile& proj : m_Projectiles)
    {
        proj.Update(elapsedSec);
    }

    if (player.DoesWantToThrow())
    {
        m_Projectiles.emplace_back(Lance(player.GetCenterPosition(), player.IsFacingRight()));
    }

    for (Enemy* zomb : m_Enemies)
    {
        for (Projectile& proj : m_Projectiles)
        {
            if (utils::IsOverlapping(zomb->GetHitbox(), proj.GetHitbox()))
            {
                zomb->Kill();
                proj.Kill();
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

    for (const Projectile& proj : m_Projectiles)
    {
        proj.Draw();
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

void EntityManager::SpawnProjectile(const Projectile& projectile)
{
    m_Projectiles.push_back(projectile);
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

    for (int i = 0; i < m_Projectiles.size(); ++i)
    {
        if (m_Projectiles[i].isDead())
        {
            m_Projectiles.erase(m_Projectiles.begin() + i);
            --i;
        }
    }
}
void EntityManager::SetLevel(const Level* pLevel)
{
    m_pLevel = pLevel;
}
