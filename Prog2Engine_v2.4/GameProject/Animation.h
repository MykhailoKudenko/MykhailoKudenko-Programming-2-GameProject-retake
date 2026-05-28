#pragma once
#include "Texture.h"

class Animation
{
public:
	Animation(const std::string& texturePath, int nrFrames, float frameSec, bool loop = true);
	//rule of 5
	Animation(const Animation&) = delete;
	Animation& operator=(const Animation&) = delete;
	Animation(Animation&&) = delete;
	Animation& operator=(Animation&&) = delete;

	void Update(float elapsedSec);
	void Draw(const Rectf& destRect, bool isMirrored) const;
	void DrawFrame(const Rectf& destRect, int frame, bool isMirrored) const;

	void Reset();
	void Play();
	void Pause();
	void Stop();

	bool IsFinished() const;
	bool IsPlaying() const;

	void SetLooping(bool loop);
	void SetFrame(int frame);
	int GetCurrentFrame() const;

	float GetFrameWidth() const;
	float GetFrameHeight() const;

	void DrawAtTime(const Rectf& destRect, float time, bool isMirrored) const;
	int GetFrameAtTime(float time) const;

	bool IsTimeFinished(float time) const;
	float GetTotalDuration() const;

private:
	Texture m_Texture;

	int m_NrFrames{};
	int m_CurrentFrame{};

	float m_FrameSec{};
	float m_AccumulatedSec{};

	bool m_IsLooping{ true };
	bool m_IsPlaying{ true };
	bool m_IsFinished{ false };
};
