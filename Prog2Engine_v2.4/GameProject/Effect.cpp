#include "pch.h"
#include "Effect.h"
#include "TextureManager.h"

namespace{
    Animation GetAnimation(Effect::EffectType type)
    {
        switch (type)
        {
        case Effect::EffectType::Blood:
            return Animation("BloodDeath.png", 3, 0.13f, false);
            break;
        case Effect::EffectType::Fire:
            return  Animation("FireDeath.png", 3, 0.13f, false);
            break;
        case Effect::EffectType::Blink:
            return  Animation("SparklingHit.png", 2, 0.13f, false);
            break;
        default:
            return Animation();
        }
    }
}

Effect::Effect(const Vector2f& pos, EffectType type, bool isMirrored)
    : m_Position{ pos }
    , m_IsMirrored{ isMirrored },
    m_MyAnimation{GetAnimation(type)}
{
   
}


void Effect::Update(float elapsedSec)
{
    m_MyAnimation.Update(elapsedSec);
}

void Effect::Draw() const
{

    float width = m_MyAnimation.GetFrameWidth();
    float height = m_MyAnimation.GetFrameHeight();

    Vector2f drawPos
    {
        m_Position.x - width / 2.f,
        m_Position.y - height / 2.f
    };
    m_MyAnimation.Draw(Rectf{ drawPos.x, drawPos.y, width, height }, m_IsMirrored);
}

bool Effect::IsFinished() const
{
    return m_MyAnimation.IsFinished();
}


