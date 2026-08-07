#pragma once
#include "Texture.h"

class Animation final
{
public:
	Animation(const std::string& texturePath, int nrFrames, float frameSec, bool loop = true);
	

	void Update(float elapsedSec);
	void Draw(const Rectf& destRect, bool isMirrored, bool stretchToFit = true) const;

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

private:
	const Texture* m_Texture{nullptr};

	int m_NrFrames{};
	int m_CurrentFrame{};

	float m_FrameSec{};
	float m_AccumulatedSec{};

	bool m_IsLooping{ true };
	bool m_IsPlaying{ true };
	bool m_IsFinished{ false };
};
