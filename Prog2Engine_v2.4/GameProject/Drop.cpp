#include "pch.h"
#include "Drop.h"
#include "utils.h"
#include "TextureManager.h"
// static textures

Drop::Drop(const Vector2f& pos, DropType type)
	: m_Collider{ pos.x, pos.y, 10.f, 10.f }
	, m_Type{ type }
{
	
	switch (m_Type)
	{
	case DropType::Lance: 
		pTexture = TextureManager::GetInstance().GetTexture("Lance.png");
		break;
	case DropType::Knife:
		pTexture = TextureManager::GetInstance().GetTexture("Knife.png");
		break;
	case DropType::Torch:
		pTexture = TextureManager::GetInstance().GetTexture("Torch.png");
		break;
	case DropType::Doll:
		pTexture = TextureManager::GetInstance().GetTexture("Doll.png");
		break;
	case DropType::MoneyBag:
		pTexture = TextureManager::GetInstance().GetTexture("MoneyBag.png");
		break;
	}
	if (pTexture != nullptr)
	{
		m_Collider.width = pTexture->GetWidth();
		m_Collider.height = pTexture->GetHeight();
	}
}



void Drop::Update(float elapsedSec)
{
	if (m_Mystate == State::Falling && !utils::CheckSideCollision(*m_pVertices, m_Collider, utils::Side::bottom))
	{
		m_Collider.bottom += m_Gravity * elapsedSec;
	}
	else
	{
		m_Mystate = State::NotFalling;
	}
}

void Drop::Draw() const
{
	if (pTexture != nullptr)
	{
		pTexture->Draw(Vector2f{ m_Collider.left, m_Collider.bottom });
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

void Drop::SetWorld(const std::vector<std::vector<Vector2f>>* vertices)
{
	m_pVertices = vertices;
}
