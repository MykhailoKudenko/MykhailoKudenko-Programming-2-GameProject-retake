#pragma once
#include "Player.h"

class HUD final
{
public:
    HUD();
    
    void Update(float elapsedSec);
    void Draw(int score, Player::PlayerWeapon weapon) const;

    void ResetTimer();
    bool DidTimerFinish() const;
private:

    float DrawNumber(int number, Vector2f location) const;
    void InitNumbersAndSymbols();

    float m_TimeLeft;
    const float m_StartingTime;
    const Texture* m_pLanceTexture;
    const Texture* m_pKnifeTexture;
    const Texture* m_pTorchTexture;

    const Texture* m_pVpLabel;
    const Texture* m_pTimeLabel;

    const Texture* m_pDigitTextures[10];
    const Texture* m_pcolonSymbol;
};
