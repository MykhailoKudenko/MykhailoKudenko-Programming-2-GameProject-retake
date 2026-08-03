#include "pch.h"
#include "Effect.h"
#include "TextureManager.h"

Effect::Effect(const Vector2f& pos, EffectType type, bool isMirrored)
    : m_Position{ pos }
    , m_Type{ type }
    , m_IsMirrored{ isMirrored }
{
    switch (m_Type)
    {
    case EffectType::Blood:
        m_pMyAnimation = new Animation("BloodDeath.png", 3, 0.13f, false);
        break;
    case EffectType::Fire:
        m_pMyAnimation = new Animation("FireDeath.png", 3, 0.13f, false);
        break;
    case EffectType::Blink:
        m_pMyAnimation = new Animation("SparklingHit.png", 2, 0.13f, false);
        break;
    }
}

void Effect::Update(float elapsedSec)
{
    m_pMyAnimation->Update(elapsedSec);
}

void Effect::Draw() const
{

    float width = m_pMyAnimation->GetFrameWidth();
    float height = m_pMyAnimation->GetFrameHeight();

    Vector2f drawPos
    {
        m_Position.x - width / 2.f,
        m_Position.y - height / 2.f
    };
    m_pMyAnimation->Draw(Rectf{ drawPos.x, drawPos.y, width, height }, m_IsMirrored);
}

bool Effect::IsFinished() const
{
    return m_pMyAnimation->IsFinished();
}


