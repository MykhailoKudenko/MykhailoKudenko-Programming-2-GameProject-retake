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

    void Update(float elapsedSec);
    void Draw() const;
    bool IsFinished() const;

    static void InitializeAssets();
    static void FreeAssets();

private:
    Vector2f m_Position;
    EffectType m_Type;
    float m_AnimTime{ 0.f };
    bool m_IsMirrored{ false };

    static Animation* m_pBloodAnimation;
    static Animation* m_pFireAnimation;
    static Animation* m_pBlinkAnimation;
};