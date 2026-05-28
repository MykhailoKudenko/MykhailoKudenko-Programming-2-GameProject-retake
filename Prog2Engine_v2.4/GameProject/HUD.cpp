#include "pch.h"
#include "HUD.h"

// static init
Texture* HUD::m_pLanceTexture{ nullptr };
Texture* HUD::m_pKnifeTexture{ nullptr };
Texture* HUD::m_pTorchTexture{ nullptr };
int HUD::m_InstanceCount{ 0 };

HUD::HUD()
    : m_TimeLeft{ 180.f }
{
    --m_InstanceCount;
    if (m_pLanceTexture == nullptr)
    {
        m_pLanceTexture = new Texture("Lance.png");
    }

    if (m_pKnifeTexture == nullptr)
    {
        m_pKnifeTexture = new Texture("Knife.png");
    }

    if (m_pTorchTexture == nullptr)
    {
        m_pTorchTexture = new Texture("Torch.png");
    }
}

HUD::~HUD()
{
    --m_InstanceCount;

    if (m_InstanceCount <= 0)
    {
        delete m_pLanceTexture;
        m_pLanceTexture = nullptr;

        delete m_pKnifeTexture;
        m_pKnifeTexture = nullptr;

        delete m_pTorchTexture;
        m_pTorchTexture = nullptr;

        m_InstanceCount = 0;
    }
}

void HUD::Update(float elapsedSec)
{
    m_TimeLeft -= elapsedSec;

    if (m_TimeLeft < 0.f)
    {
        m_TimeLeft = 0.f;
    }
}

void HUD::Draw(int score, PlayerWeapon weapon) const
{
    const float screenWidth = 1024.f;
    const float screenHeight = 960.f;
    const float centerX = screenWidth / 2.f;

    Color4f PinkColor{ 243.f / 255.f, 194.f / 255.f, 191.f / 255.f, 1.0f };

    Texture vpLabel("1   VP", "LowresPixel-Regular.otf", 60, PinkColor);
    vpLabel.Draw(Vector2f{ 100.f, 885.f });

    Texture timeLabel("TIME", "LowresPixel-Regular.otf", 60, PinkColor);
    timeLabel.Draw(Vector2f{ 100.f, 775.f });

    std::string scoreText = std::to_string(score);
    Texture scoreTex(scoreText, "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });
    scoreTex.Draw(Vector2f{ 300.f, 830 });

    std::string timerText = GetTimerText();
    Texture timerTex(timerText, "LowresPixel-Regular.otf", 60, Color4f{ 1,1,1,1 });
    timerTex.Draw(Vector2f{ 100.f, 720 });

    const float weaponY = 100.f;
    const float scale = 6.0f;

    switch (weapon)
    {
    case PlayerWeapon::Lance:
        if (m_pLanceTexture)
        {
            float w = m_pLanceTexture->GetWidth();
            float h = m_pLanceTexture->GetHeight();

            Vector2f pos{
                centerX - (w * scale) / 2.f,
                weaponY
            };

            glPushMatrix();
            glTranslatef(pos.x, pos.y, 0.f);
            glScalef(scale, scale, 1.f);

            m_pLanceTexture->Draw(Vector2f{ 0.f, 0.f });

            glPopMatrix();
        }
        break;

    case PlayerWeapon::Knife:
        if (m_pKnifeTexture)
        {
            float w = m_pKnifeTexture->GetWidth();
            float h = m_pKnifeTexture->GetHeight();

            Vector2f pos{
                centerX - (w * scale) / 2.f,
                weaponY
            };

            glPushMatrix();
            glTranslatef(pos.x, pos.y, 0.f);
            glScalef(scale, scale, 1.f);

            m_pKnifeTexture->Draw(Vector2f{ 0.f, 0.f });

            glPopMatrix();
        }
        break;

    case PlayerWeapon::Tourch:
        if (m_pTorchTexture)
        {
            float w = m_pTorchTexture->GetWidth();

            Vector2f pos{
                centerX - (w * scale) / 2.f,
                weaponY
            };

            glPushMatrix();
            glTranslatef(pos.x, pos.y, 0.f);
            glScalef(scale, scale, 1.f);

            m_pTorchTexture->Draw(Vector2f{ 0.f, 0.f });

            glPopMatrix();
        }
        break;
    }
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
    m_TimeLeft = 180.f;
}

bool HUD::DidTimerFinish()
{
    if (m_TimeLeft > 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}