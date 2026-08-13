#include "pch.h"
#include "SoundManager.h"

SoundManager::SoundManager()
    : m_Level1Music("Sounds/BackGroundMusic.mp3")
    , m_Throw("Sounds/Throw.mp3")
    , m_PickUp("Sounds/PickUp.mp3")
    , m_FireDead("Sounds/FireDead.mp3")
    , m_FlyingKnightSound("Sounds/FlyingKnight.mp3")
    , m_GhostSound("Sounds/Ghost.mp3")
    , m_Death("Sounds/Death.mp3")
    , m_LevelComplete("Sounds/LevelComplete.mp3")
{
}


SoundManager& SoundManager::GetInstance()
{
    static SoundManager instance;

    return instance;
}


void SoundManager::PlayMusic()
{
    if (m_Level1Music.IsLoaded())
    {
        m_Level1Music.Play(true);
    }
}

void SoundManager::StopMusic()
{
    SoundStream::Stop();
}

void SoundManager::PlayEffect(SoundManager::SFX effect)
{
    switch (effect)
    {
    case SFX::Throw:
        if (m_Throw.IsLoaded())
        {
            m_Throw.Play(0);
        }
        break;

    case SFX::PickUp:
        if (m_PickUp.IsLoaded())
        {
            m_PickUp.Play(0);
        }
        break;

    case SFX::FireDead:
        if (m_FireDead.IsLoaded())
        {
            m_FireDead.Play(0);
        }
        break;

    case SFX::FlyingKnight:
        if (m_FlyingKnightSound.IsLoaded())
        {
            m_FlyingKnightSound.Play(0);
        }
        break;

    case SFX::Ghost:
        if (m_GhostSound.IsLoaded())
        {
            m_GhostSound.Play(0);
        }
        break;

    case SFX::Death:
        if (m_Death.IsLoaded())
        {
            m_Death.Play(0);
        }
        break;

    case SFX::LevelComplete:
        if (m_LevelComplete.IsLoaded())
        {
            m_LevelComplete.Play(0);
        }
        break;
    }
}

void SoundManager::SetMusicVolume(int volume)
{
    m_Level1Music.SetVolume(volume);
}

void SoundManager::SetEffectVolume(int volume)
{
    m_Throw.SetVolume(volume);
    m_PickUp.SetVolume(volume);
    m_FireDead.SetVolume(volume);
    m_FlyingKnightSound.SetVolume(volume);
    m_GhostSound.SetVolume(volume);
    m_Death.SetVolume(volume);
    m_LevelComplete.SetVolume(volume);
}