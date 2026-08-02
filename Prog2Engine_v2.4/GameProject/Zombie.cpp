#include "pch.h"
#include "Zombie.h"
#include "utils.h"

Animation* Zombie::m_pWalkAnimation{ nullptr };
Animation* Zombie::m_pSpawnAnimation{ nullptr };
int Zombie::m_InstanceCount{ 0 };

Zombie::Zombie(Vector2f startPos, bool facingRight)
	: Enemy(Rectf{ startPos.x, startPos.y, 19, 27 })
{
	m_IsFacingRight = facingRight;
	m_Speed = 30.f;
	if (!facingRight)
	{
		m_Speed *= -1.f;
	}

	++m_InstanceCount;

	if (m_pWalkAnimation == nullptr)
	{
		m_pWalkAnimation = new Animation("ZombieWalk.png", 2, 0.13f, true);
	}

	if (m_pSpawnAnimation == nullptr)
	{
		m_pSpawnAnimation = new Animation("ZombieSpawn.png", 3, 0.39f, false);
	}
}

Zombie::~Zombie()
{
	--m_InstanceCount;

	if (m_InstanceCount <= 0)
	{
		delete m_pWalkAnimation;
		m_pWalkAnimation = nullptr;

		delete m_pSpawnAnimation;
		m_pSpawnAnimation = nullptr;

		m_InstanceCount = 0;
	}
}

void Zombie::Update(float elapsedSec)
{
	if (m_pVertices == nullptr)
	{
		return;
	}

	m_AnimTime += elapsedSec;

	if (m_State == ZombieState::Spawning)
	{
		if (m_pSpawnAnimation != nullptr && m_pSpawnAnimation->IsTimeFinished(m_AnimTime))
		{
			m_State = ZombieState::Walking;
			m_AnimTime = 0.f;
		}
		return;
	}


	m_Velocity.x = m_Speed * elapsedSec;
	m_Velocity.y = m_Gravity * elapsedSec;

	// collisions right/left
	utils::HitInfo myInfoTopSide{};
	utils::HitInfo myInfoBottomSide{};

	bool hitWallOnX = false;

	if (m_Velocity.x > 0)
	{

		hitWallOnX = utils::CheckSideCollision(*m_pVertices, m_Collider, utils::Side::right, m_Velocity.x);



		if (!hitWallOnX)
		{
			m_Collider.left += m_Velocity.x;
		}
		else
		{
			m_Speed *= -1;
			m_IsFacingRight = (m_Speed > 0);
		}
	}
	else if (m_Velocity.x < 0)
	{
		bool hitWallOnX = utils::CheckSideCollision(*m_pVertices, m_Collider, utils::Side::left, -m_Velocity.x);


		if (!hitWallOnX)
		{
			m_Collider.left += m_Velocity.x;
		}
		else
		{
			m_Speed *= -1;
			m_IsFacingRight = (m_Speed > 0);

		}
	}
	utils::HitInfo gravityHit{};
	if (!utils::CheckSideCollision(*m_pVertices, m_Collider, utils::Side::bottom, 2.f, &gravityHit))
	{
		m_Collider.bottom += m_Velocity.y;
	}
	else
	{
		m_Velocity.y = 0;
		m_Collider.bottom = gravityHit.intersectPoint.y;
	}
}



void Zombie::Draw() const
{
	if (m_State == ZombieState::Spawning)
	{
		if (m_pSpawnAnimation != nullptr)
		{
			m_pSpawnAnimation->DrawAtTime(Rectf{ m_Collider.left, m_Collider.bottom, m_pSpawnAnimation->GetFrameWidth(), m_pSpawnAnimation->GetFrameHeight() }, m_AnimTime, m_IsFacingRight);
		}
	}
	else if (m_State == ZombieState::Walking)
	{
		if (m_pWalkAnimation != nullptr)
		{
			m_pWalkAnimation->DrawAtTime(Rectf{ m_Collider.left, m_Collider.bottom, m_pWalkAnimation->GetFrameWidth(), m_pWalkAnimation->GetFrameHeight()}, m_AnimTime, m_IsFacingRight);
		}
	}

}
void Zombie::SetWorld(const std::vector<std::vector<Vector2f>>* vertices)
{
	m_pVertices = vertices;
}


bool Zombie::IsSpawning() const
{
	if (m_State == ZombieState::Spawning)
	{
		return true;
	}
	else
	{
		return false;
	}
}