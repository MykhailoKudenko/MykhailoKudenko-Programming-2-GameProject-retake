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

    ~Effect();
    Effect(const Effect& other) = delete;
    Effect& operator=(const Effect& other) = delete;
    Effect(Effect&& other) noexcept = delete;
    Effect& operator=(Effect&& other) noexcept = delete;

    void Update(float elapsedSec);
    void Draw() const;
    bool IsFinished() const;

private:
    Vector2f m_Position;
    EffectType m_Type;
    bool m_IsMirrored{ false };

    Animation* m_pMyAnimation;

};