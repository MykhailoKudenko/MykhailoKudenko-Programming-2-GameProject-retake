#pragma once
#include "Animation.h"

class Effect final
{
public:
    enum class EffectType
    {
        Blood,
        Fire,
        Blink
    };

    Effect(const Vector2f& pos, EffectType type, bool isMirrored = false);

    void Update(float elapsedSec);
    void Draw() const;
    bool IsFinished() const;

private:
    Animation GetAnimation(EffectType type);

    Vector2f m_Position;
    bool m_IsMirrored{ false };

    Animation m_MyAnimation;

};