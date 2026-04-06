#include "pch.h"
#include "Effect.h"

Animation* Effect::m_pBloodAnimation{ nullptr };
Animation* Effect::m_pFireAnimation{ nullptr };
Animation* Effect::m_pBlinkAnimation{ nullptr };

Effect::Effect(const Vector2f& pos, EffectType type, bool isMirrored)
    : m_Position{ pos }
    , m_Type{ type }
    , m_IsMirrored{ isMirrored }
{
}

void Effect::Update(float elapsedSec)
{
    m_AnimTime += elapsedSec;
}

void Effect::Draw() const
{
    Animation* anim{ nullptr };

    switch (m_Type)
    {
    case EffectType::Blood:
        anim = m_pBloodAnimation;
        break;
    case EffectType::Fire:
        anim = m_pFireAnimation;
        break;
    case EffectType::Blink:
        anim = m_pBlinkAnimation;
        break;
    }

    if (anim == nullptr)
        return;

    // Center the animation on position
    float width = anim->GetFrameWidth();
    float height = anim->GetFrameHeight();

    Vector2f drawPos
    {
        m_Position.x - width / 2.f,
        m_Position.y - height / 2.f
    };
    anim->DrawAtTime(Rectf{ drawPos.x, drawPos.y, width, height }, m_AnimTime, m_IsMirrored );
}

bool Effect::IsFinished() const
{
    Animation* anim{ nullptr };

    switch (m_Type)
    {
    case EffectType::Blood:
        anim = m_pBloodAnimation;
        break;
    case EffectType::Fire:
        anim = m_pFireAnimation;
        break;
    case EffectType::Blink:
        anim = m_pBlinkAnimation;
        break;
    }

    if (anim == nullptr)
        return true;

    return anim->IsTimeFinished(m_AnimTime);
}

void Effect::InitializeAssets()
{
    if (m_pBloodAnimation == nullptr)
    {
        m_pBloodAnimation = new Animation("BloodDeath.png", 3, 0.13f, false);
    }

    if (m_pFireAnimation == nullptr)
    {
        m_pFireAnimation = new Animation("FireDeath.png", 3, 0.13f, false);
    }

    if (m_pBlinkAnimation == nullptr)
    {
        m_pBlinkAnimation = new Animation("Sparklinghit.png", 2, 0.13f, false);
    }
}

void Effect::FreeAssets()
{
    delete m_pBloodAnimation;
    m_pBloodAnimation = nullptr;

    delete m_pFireAnimation;
    m_pFireAnimation = nullptr;

    delete m_pBlinkAnimation;
    m_pBlinkAnimation = nullptr;
}