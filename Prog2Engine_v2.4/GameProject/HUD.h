#pragma once
#include "Player.h"

class HUD final
{
public:
    HUD();
    ~HUD();
    


    void Update(float elapsedSec);
    void Draw(int score, PlayerWeapon weapon) const;

    void ResetTimer();
    bool DidTimerFinish() const;
private:
    std::string GetTimerText() const;

    float m_TimeLeft{ 180.f };

    static Texture* m_pLanceTexture;
    static Texture* m_pKnifeTexture;
    static Texture* m_pTorchTexture;
    static int m_InstanceCount;

};
