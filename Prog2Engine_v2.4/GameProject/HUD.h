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
    std::string GetTimerText() const;

    float DrawNumber(int number, Vector2f location) const;
    void IntitNumbersAndSymbols();

    float m_TimeLeft;
    const float m_StartingTime;
    const Texture* m_pLanceTexture;
    const Texture* m_pKnifeTexture;
    const Texture* m_pTorchTexture;

    const Texture* m_pVpLabel;
    const Texture* m_pTimeLabel;

};
