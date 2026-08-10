#include "pch.h"
#include "Drop.h"
#include "utils.h"
#include "TextureManager.h"

Drop::Drop(const Vector2f& pos, DropType type, const std::vector<std::vector<Vector2f>>* vertices)
	: m_Collider{ pos.x, pos.y, 10.f, 10.f }
	, m_Type{ type },
	m_IsDead{ false },
	m_pTexture{ nullptr },
	m_Mystate{ State::Falling },
	m_pVertices{vertices}
{
	
	switch (m_Type)
	{
	case DropType::Lance: 
		m_pTexture = TextureManager::GetInstance().GetTexture("Lance.png");
		break;
	case DropType::Knife:
		m_pTexture = TextureManager::GetInstance().GetTexture("Knife.png");
		break;
	case DropType::Torch:
		m_pTexture = TextureManager::GetInstance().GetTexture("Torch.png");
		break;
	case DropType::Doll:
		m_pTexture = TextureManager::GetInstance().GetTexture("Doll.png");
		break;
	case DropType::MoneyBag:
		m_pTexture = TextureManager::GetInstance().GetTexture("MoneyBag.png");
		break;
	}
	if (m_pTexture != nullptr)
	{
		m_Collider.width = m_pTexture->GetWidth();
		m_Collider.height = m_pTexture->GetHeight();
	}
}



void Drop::Update(float elapsedSec)
{
	if (m_pVertices == nullptr)
	{
		return;
	}
	if (m_Mystate == State::Falling && !utils::CheckSideCollision(*m_pVertices, m_Collider, utils::Side::bottom))
	{
		m_Collider.bottom += utils::g_Gravity * elapsedSec;
	}
	else
	{
		m_Mystate = State::NotFalling;
	}
}

void Drop::Draw() const
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Draw(Vector2f{ m_Collider.left, m_Collider.bottom });
	}
}

Rectf Drop::GetHitbox() const
{
	return m_Collider;
}

Drop::DropType Drop::GetType() const
{
	return m_Type;
}

bool Drop::IsDead() const
{
	return m_IsDead;
}

void Drop::Kill()
{
	m_IsDead = true;
}
