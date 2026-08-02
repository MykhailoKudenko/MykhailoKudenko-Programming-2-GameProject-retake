#include "pch.h"
#include "HUD.h"
#include "TextureManager.h"


HUD::HUD()
    : m_TimeLeft{ 180.f }
{
    m_pLanceTexture = TextureManager::GetInstance().GetTexture("Lance.png");
    m_pKnifeTexture = TextureManager::GetInstance().GetTexture("Knife.png");
    m_pTorchTexture = TextureManager::GetInstance().GetTexture("Torch.png");


    m_pVpLabel = TextureManager::GetInstance().GetTextTexture("1   VP", "LowresPixel-Regular.otf", 60, Color4f{ 243.f / 255.f, 194.f / 255.f, 191.f / 255.f, 1.0f });
    m_pTimeLabel = TextureManager::GetInstance().GetTextTexture("TIME", "LowresPixel-Regular.otf", 60, Color4f{ 243.f / 255.f, 194.f / 255.f, 191.f / 255.f, 1.0f });
}

void HUD::Update(float elapsedSec)
{
    m_TimeLeft -= elapsedSec;

    if (m_TimeLeft < 0.f)
    {
        m_TimeLeft = 0.f;
    }
}

void HUD::Draw(int score, Player::PlayerWeapon weapon) const
{
    

    m_pVpLabel->Draw(Vector2f{ 100.f, 885.f });

    m_pTimeLabel->Draw(Vector2f{ 100.f, 775.f });

    std::string scoreText = std::to_string(score);
    const Texture* scoreTexture = TextureManager::GetInstance().GetTextTexture(scoreText, "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });
    scoreTexture->Draw(Vector2f{ 300.f, 830 });

    std::string timerText = GetTimerText();
    const Texture* timerTexture = TextureManager::GetInstance().GetTextTexture(timerText, "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });
    timerTexture->Draw(Vector2f{ 100.f, 720 });

    //weapon placement
    const float screenWidth = 1024.f;
    const float screenHeight = 960.f;
    const float centerX = screenWidth / 2.f;
    const float weaponY = 100.f;
    const float scale = 6.0f;

    const Texture* weaponToDraw = nullptr;


    switch (weapon)
    {
    case Player::PlayerWeapon::Lance:
        if (m_pLanceTexture)
        {
            weaponToDraw = m_pLanceTexture;
        }
        break;

    case Player::PlayerWeapon::Knife:
        if (m_pKnifeTexture)
        {
            weaponToDraw = m_pKnifeTexture;
        }
        break;

    case Player::PlayerWeapon::Torch:
        if (m_pTorchTexture)
        {
            weaponToDraw = m_pTorchTexture;
        }
        break;
    }

    if (weaponToDraw == nullptr)
    {
        return;
    }
    float w = weaponToDraw->GetWidth();
    float h = weaponToDraw->GetHeight();

    Vector2f pos{
        centerX - (w * scale) / 2.f,
        weaponY
    };

    glPushMatrix();
    glTranslatef(pos.x, pos.y, 0.f);
    glScalef(scale, scale, 1.f);

    weaponToDraw->Draw(Vector2f{ 0.f, 0.f });

    glPopMatrix();
}

std::string HUD::GetTimerText() const
{
    int totalSeconds = static_cast<int>(m_TimeLeft);
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    std::string secStr = (seconds < 10)
        ? "0" + std::to_string(seconds)
        : std::to_string(seconds);

    return std::to_string(minutes) + ":" + secStr;
}

void HUD::ResetTimer()
{
    m_TimeLeft = 240.f;
}

bool HUD::DidTimerFinish() const
{
    return m_TimeLeft < 0;
    
}