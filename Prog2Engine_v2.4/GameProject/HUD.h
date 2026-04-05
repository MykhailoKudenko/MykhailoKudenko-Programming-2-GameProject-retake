#pragma once
#include "Player.h"

class HUD
{
public:
    HUD();

    void Update(float elapsedSec);
    void Draw(int score, PlayerWeapon weapon) const;

    static void InitializeAssets();
    static void FreeAssets();

    void ResetTimer();

private:
    std::string GetTimerText() const;

    float m_TimeLeft{ 180.f };

    static Texture* m_pLanceTexture;
    static Texture* m_pKnifeTexture;
    static Texture* m_pTorchTexture;
};
