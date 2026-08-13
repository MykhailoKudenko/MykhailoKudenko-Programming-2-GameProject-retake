#pragma once
#include "SoundStream.h"
#include "SoundEffect.h"



class SoundManager final
{
public:

    static SoundManager& GetInstance();

    ~SoundManager() = default;
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;
    SoundManager(SoundManager&&) = delete;
    SoundManager& operator=(SoundManager&&) = delete;

    enum class SFX
    {
        None,
        Throw,
        PickUp,
        FireDead,
        FlyingKnight,
        Ghost,
        Death,
        LevelComplete
    };


    void PlayMusic();
    void StopMusic();

    void PlayEffect(SoundManager::SFX effect);

    void SetMusicVolume(int volume);
    void SetEffectVolume(int volume);

private:
    SoundManager();

    SoundStream m_Level1Music;

    SoundEffect m_Throw;
    SoundEffect m_PickUp;

    SoundEffect m_FireDead;

    SoundEffect m_FlyingKnightSound;

    SoundEffect m_GhostSound;

    SoundEffect m_Death;
    SoundEffect m_LevelComplete;
};