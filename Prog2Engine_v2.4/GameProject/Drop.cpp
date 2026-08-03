#include "pch.h"
#include "Drop.h"
#include "utils.h"
#include "TextureManager.h"
// static textures

Drop::Drop(const Vector2f& pos, DropType type)
	: m_Collider{ pos.x, pos.y, 10.f, 10.f }
	, m_Type{ type }
{
	

	m_pLanceTexture = TextureManager::GetInstance().GetTexture("Lance.png");
	m_pKnifeTexture = TextureManager::GetInstance().GetTexture("Knife.png");
	m_pTorchTexture = TextureManager::GetInstance().GetTexture("Torch.png");
	m_pDollTexture = TextureManager::GetInstance().GetTexture("Doll.png");
	m_pMoneyBagTexture = TextureManager::GetInstance().GetTexture("MoneyBag.png");

	const Texture* pTexture = GetTexture();

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
	const Texture* tex = GetTexture();

	if (tex != nullptr)
	{
		tex->Draw(Vector2f{ m_Collider.left, m_Collider.bottom });
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

const Texture* Drop::GetTexture() const
{
	switch (m_Type)
	{
	case DropType::Lance: return m_pLanceTexture;
	case DropType::Knife: return m_pKnifeTexture;
	case DropType::Torch: return m_pTorchTexture;
	case DropType::Doll: return m_pDollTexture;
	case DropType::MoneyBag: return m_pMoneyBagTexture;
	default: return nullptr;
	}
}

