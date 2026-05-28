#pragma once
#include "SoundStream.h"
#include "SoundEffect.h"

enum class SFX
{
    Throw,
    PickUp,
    FireDead,
    FlyingKnight,
    Ghost,
    Death,
    LevelComplete
};

class SoundManager
{
public:
    SoundManager();
    ~SoundManager() = default;

    //rule of 5
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;
    SoundManager(SoundManager&&) = delete;
    SoundManager& operator=(SoundManager&&) = delete;

    void PlayMusic();
    void StopMusic();

    void PlayEffect(SFX effect);

    void SetMusicVolume(int volume);
    void SetEffectVolume(int volume);

private:
    SoundStream m_Level1Music;

    SoundEffect m_Throw;
    SoundEffect m_PickUp;

    SoundEffect m_FireDead;

    SoundEffect m_FlyingKnightSound;

    SoundEffect m_GhostSound;

    SoundEffect m_Death;
    SoundEffect m_LevelComplete;
};