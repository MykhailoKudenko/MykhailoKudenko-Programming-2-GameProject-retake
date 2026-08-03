#include "pch.h"
#include "HUD.h"
#include "TextureManager.h"


HUD::HUD()
    : m_StartingTime{ 240.f },
    m_TimeLeft{ 240.f }
{
    m_pLanceTexture = TextureManager::GetInstance().GetTexture("Lance.png");
    m_pKnifeTexture = TextureManager::GetInstance().GetTexture("Knife.png");
    m_pTorchTexture = TextureManager::GetInstance().GetTexture("Torch.png");


    m_pVpLabel = TextureManager::GetInstance().GetTextTexture("1   VP", "LowresPixel-Regular.otf", 60, Color4f{ 243.f / 255.f, 194.f / 255.f, 191.f / 255.f, 1.0f });
    m_pTimeLabel = TextureManager::GetInstance().GetTextTexture("TIME", "LowresPixel-Regular.otf", 60, Color4f{ 243.f / 255.f, 194.f / 255.f, 191.f / 255.f, 1.0f });

    IntitNumbersAndSymbols();
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

    DrawNumber(score, Vector2f{ 300.f, 830 });

    int totalSeconds = static_cast<int>(m_TimeLeft);
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    int displacement = 0;

    displacement += DrawNumber(minutes, Vector2f{ 100.f, 720 });
        
    const Texture* symbolTexture = TextureManager::GetInstance().GetTextTexture(":", "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });
    symbolTexture->Draw(Vector2f{ 100.f + displacement, 720 });
    displacement += symbolTexture->GetWidth();

     DrawNumber(seconds, Vector2f{ 100.f+ displacement, 720 });

    //weapon placement
    const float screenWidth = 1024.f;
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
    m_TimeLeft = m_StartingTime;
}

bool HUD::DidTimerFinish() const
{
    return m_TimeLeft <= 0.f;
    
}

float HUD::DrawNumber(int number, Vector2f location) const
{
    std::string numberString = std::to_string(number);

    float displacement = 0;

    for (char c : numberString)
    {
        std::string s{ c };
        const Texture* CurrentNumberTexture = TextureManager::GetInstance().GetTextTexture(s, "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });
        CurrentNumberTexture->Draw(Vector2f{location.x+ displacement, location.y});
        displacement += CurrentNumberTexture->GetWidth();

    }
    return displacement;
}

void HUD::IntitNumbersAndSymbols()
{
    TextureManager::GetInstance().GetTextTexture("0", "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });
    TextureManager::GetInstance().GetTextTexture("1", "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });
    TextureManager::GetInstance().GetTextTexture("2", "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });
    TextureManager::GetInstance().GetTextTexture("3", "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });
    TextureManager::GetInstance().GetTextTexture("4", "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });
    TextureManager::GetInstance().GetTextTexture("5", "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });
    TextureManager::GetInstance().GetTextTexture("6", "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });
    TextureManager::GetInstance().GetTextTexture("7", "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });
    TextureManager::GetInstance().GetTextTexture("8", "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });
    TextureManager::GetInstance().GetTextTexture("9", "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });

    TextureManager::GetInstance().GetTextTexture(":", "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });


}