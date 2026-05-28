#pragma once
#include "Animation.h"

class Effect
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
    //rule of 5
    Effect(const Effect&) = delete;
    Effect& operator=(const Effect&) = delete;

    Effect(Effect&&) = delete;
    Effect& operator=(Effect&&) = delete;

    void Update(float elapsedSec);
    void Draw() const;
    bool IsFinished() const;

private:
    Vector2f m_Position;
    EffectType m_Type;
    float m_AnimTime{ 0.f };
    bool m_IsMirrored{ false };

    static Animation* m_pBloodAnimation;
    static Animation* m_pFireAnimation;
    static Animation* m_pBlinkAnimation;
    static int m_InstanceCount;

};