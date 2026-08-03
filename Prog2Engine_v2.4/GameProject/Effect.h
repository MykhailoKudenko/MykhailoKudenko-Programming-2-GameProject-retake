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
    Vector2f m_Position;
    EffectType m_Type;
    bool m_IsMirrored{ false };

    Animation* m_pMyAnimation;

};