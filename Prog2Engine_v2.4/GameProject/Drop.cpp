#include "pch.h"
#include "Drop.h"
#include "utils.h"

// static textures
Texture* Drop::m_pLanceTexture{ nullptr };
Texture* Drop::m_pKnifeTexture{ nullptr };
Texture* Drop::m_pTorchTexture{ nullptr };
Texture* Drop::m_pDollTexture{ nullptr };
Texture* Drop::m_pMoneyBagTexture{ nullptr };
int Drop::m_InstanceCount{ 0 };

Drop::Drop(const Vector2f& pos, PickupType type)
	: m_Collider{ pos.x, pos.y, 10.f, 10.f }
	, m_Type{ type }
{
	Texture* pTexture = GetTexture();

	if (pTexture != nullptr)
	{
		m_Collider.width = pTexture->GetWidth();
		m_Collider.height = pTexture->GetHeight();
	}

	++m_InstanceCount;

	if (m_pLanceTexture == nullptr)
		m_pLanceTexture = new Texture("Lance.png");

	if (m_pKnifeTexture == nullptr)
		m_pKnifeTexture = new Texture("Knife.png");

	if (m_pTorchTexture == nullptr)
		m_pTorchTexture = new Texture("Torch.png");

	if (m_pDollTexture == nullptr)
		m_pDollTexture = new Texture("Doll.png");

	if (m_pMoneyBagTexture == nullptr)
		m_pMoneyBagTexture = new Texture("MoneyBag.png");

}
Drop::~Drop()
{
	--m_InstanceCount;

	if (m_InstanceCount <= 0)
	{
		delete m_pLanceTexture;
		delete m_pKnifeTexture;
		delete m_pTorchTexture;
		delete m_pDollTexture;
		delete m_pMoneyBagTexture;

		m_pLanceTexture = nullptr;
		m_pKnifeTexture = nullptr;
		m_pTorchTexture = nullptr;
		m_pDollTexture = nullptr;
		m_pMoneyBagTexture = nullptr;

		m_InstanceCount = 0;
	}
}

Drop::Drop(const Drop& other) //copy constr
	: m_pVertices{ other.m_pVertices }
	, m_Collider{ other.m_Collider }
	, m_Type{ other.m_Type }
	, m_IsDead{ other.m_IsDead }
	, m_Mystate{ other.m_Mystate }
{
	++m_InstanceCount;

	if (m_pLanceTexture == nullptr)
		m_pLanceTexture = new Texture("Lance.png");

	if (m_pKnifeTexture == nullptr)
		m_pKnifeTexture = new Texture("Knife.png");

	if (m_pTorchTexture == nullptr)
		m_pTorchTexture = new Texture("Torch.png");

	if (m_pDollTexture == nullptr)
		m_pDollTexture = new Texture("Doll.png");

	if (m_pMoneyBagTexture == nullptr)
		m_pMoneyBagTexture = new Texture("MoneyBag.png");
}

Drop& Drop::operator=(const Drop& other) //copy assign
{
	if (this == &other)
		return *this;

	m_pVertices = other.m_pVertices;
	m_Collider = other.m_Collider;
	m_Type = other.m_Type;
	m_IsDead = other.m_IsDead;
	m_Mystate = other.m_Mystate;

	return *this;
}

Drop::Drop(Drop&& other) noexcept //move constr
	: m_pVertices{ other.m_pVertices }
	, m_Collider{ other.m_Collider }
	, m_Type{ other.m_Type }
	, m_IsDead{ other.m_IsDead }
	, m_Mystate{ other.m_Mystate }
{
	++m_InstanceCount;

	other.m_pVertices = nullptr;
	other.m_IsDead = true;
}

Drop& Drop::operator=(Drop&& other) noexcept//move assign
{
	if (this == &other)
		return *this;

	m_pVertices = other.m_pVertices;
	m_Collider = other.m_Collider;
	m_Type = other.m_Type;
	m_IsDead = other.m_IsDead;
	m_Mystate = other.m_Mystate;

	other.m_pVertices = nullptr;
	other.m_IsDead = true;

	return *this;
}

void Drop::Update(float elapsedSec)
{
	if (m_Mystate == State::Falling && !CheckBottomCollision())
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
	Texture* tex = GetTexture();

	if (tex != nullptr)
	{
		tex->Draw(Vector2f{ m_Collider.left, m_Collider.bottom });
	}
}

Rectf Drop::GetHitbox() const
{
	return m_Collider;
}

PickupType Drop::GetType() const
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

bool Drop::CheckBottomCollision() const
{
	utils::HitInfo myInfoLeft{};
	utils::HitInfo myInfoRight{};

	bool hitLeft = utils::LoopOverVertecies(
		*m_pVertices,
		Vector2f{ m_Collider.left, m_Collider.bottom + m_Collider.height },
		Vector2f{ m_Collider.left, m_Collider.bottom - 1.f },
		myInfoLeft);

	bool hitRight = utils::LoopOverVertecies(
		*m_pVertices,
		Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom + m_Collider.height },
		Vector2f{ m_Collider.left + m_Collider.width, m_Collider.bottom - 1.f },
		myInfoRight);

	if (!(hitLeft || hitRight))
	{
		return false;
	}
	else
	{
		bool validLeftHit = hitLeft && myInfoLeft.intersectPoint.y <= m_Collider.bottom - 1.f + 2.f;
		bool validRightHit = hitRight && myInfoRight.intersectPoint.y <= m_Collider.bottom - 1.f + 2.f;

		if (!(validLeftHit || validRightHit))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

Texture* Drop::GetTexture() const
{
	switch (m_Type)
	{
	case PickupType::Lance: return m_pLanceTexture;
	case PickupType::Knife: return m_pKnifeTexture;
	case PickupType::Torch: return m_pTorchTexture;
	case PickupType::Doll: return m_pDollTexture;
	case PickupType::MoneyBag: return m_pMoneyBagTexture;
	default: return nullptr;
	}
}

