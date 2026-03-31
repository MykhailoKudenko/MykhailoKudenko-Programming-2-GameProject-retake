#include "pch.h"
#include "Animation.h"



Animation::Animation(const std::string& texturePath, int nrFrames, float frameSec, bool loop)
	: m_Texture{ texturePath }
	, m_NrFrames{ nrFrames }
	, m_CurrentFrame{ 0 }
	, m_FrameSec{ frameSec }
	, m_AccumulatedSec{ 0.f }
	, m_IsLooping{ loop }
	, m_IsPlaying{ true }
	, m_IsFinished{ false }
{
}

void Animation::Update(float elapsedSec)
{
	if (!m_IsPlaying || m_IsFinished || m_NrFrames <= 0)
	{
		return;
	}

	m_AccumulatedSec += elapsedSec;

	while (m_AccumulatedSec >= m_FrameSec)
	{
		m_AccumulatedSec -= m_FrameSec;
		++m_CurrentFrame;

		if (m_CurrentFrame >= m_NrFrames)
		{
			if (m_IsLooping)
			{
				m_CurrentFrame = 0;
			}
			else
			{
				m_CurrentFrame = m_NrFrames - 1;
				m_IsFinished = true;
				m_IsPlaying = false;
				break;
			}
		}
	}
}

void Animation::Draw(const Rectf& destRect, bool isMirrored) const
{
	if (m_NrFrames <= 0)
	{
		return;
	}

	const float frameWidth{ m_Texture.GetWidth() / m_NrFrames };
	const float frameHeight{ m_Texture.GetHeight() };

	const Rectf srcRect
	{
		frameWidth * m_CurrentFrame,
		0.f,
		frameWidth,
		frameHeight
	};

	glPushMatrix();

	glTranslatef(destRect.left, destRect.bottom, 0.f);

	if (isMirrored)
	{
		glTranslatef(destRect.width, 0.f, 0.f);
		glScalef(-1.f, 1.f, 1.f);
	}

	Rectf localDestRect
	{
		0.f,
		0.f,
		destRect.width,
		destRect.height
	};

	m_Texture.Draw(localDestRect, srcRect);

	glPopMatrix();
}

void Animation::DrawFrame(const Rectf& destRect, int frame, bool isMirrored) const
{
	if (m_NrFrames <= 0)
	{
		return;
	}

	// clamp frame manually
	if (frame < 0)
	{
		frame = 0;
	}
	else if (frame > m_NrFrames - 1)
	{
		frame = m_NrFrames - 1;
	}

	const float frameWidth{ m_Texture.GetWidth() / m_NrFrames };
	const float frameHeight{ m_Texture.GetHeight() };

	const Rectf srcRect
	{
		frameWidth * frame,
		0.f,
		frameWidth,
		frameHeight
	};

	glPushMatrix();

	glTranslatef(destRect.left, destRect.bottom, 0.f);

	if (isMirrored)
	{
		glTranslatef(destRect.width, 0.f, 0.f);
		glScalef(-1.f, 1.f, 1.f);
	}

	Rectf localDestRect
	{
		0.f,
		0.f,
		destRect.width,
		destRect.height
	};

	m_Texture.Draw(localDestRect, srcRect);

	glPopMatrix();
}


void Animation::Reset()
{
	m_CurrentFrame = 0;
	m_AccumulatedSec = 0.f;
	m_IsFinished = false;
	m_IsPlaying = true;
}

void Animation::Play()
{
	m_IsPlaying = true;
}

void Animation::Pause()
{
	m_IsPlaying = false;
}

void Animation::Stop()
{
	m_IsPlaying = false;
	m_CurrentFrame = 0;
	m_AccumulatedSec = 0.f;
	m_IsFinished = false;
}

bool Animation::IsFinished() const
{
	return m_IsFinished;
}

bool Animation::IsPlaying() const
{
	return m_IsPlaying;
}

void Animation::SetLooping(bool loop)
{
	m_IsLooping = loop;
}

void Animation::SetFrame(int frame)
{
	if (m_NrFrames <= 0)
	{
		return;
	}

	if (frame < 0)
	{
		frame = 0;
	}
	else if (frame > m_NrFrames - 1)
	{
		frame = m_NrFrames - 1;
	}

	m_CurrentFrame = frame;
}

int Animation::GetCurrentFrame() const
{
	return m_CurrentFrame;
}

float Animation::GetFrameWidth() const
{
	if (m_NrFrames <= 0)
	{
		return 0.f;
	}

	return m_Texture.GetWidth() / m_NrFrames;
}

float Animation::GetFrameHeight() const
{
	return m_Texture.GetHeight();
}

int Animation::GetFrameAtTime(float time) const
{
	if (m_NrFrames <= 0 || m_FrameSec <= 0.f)
	{
		return 0;
	}

	int frame = int(time / m_FrameSec);

	if (m_IsLooping)
	{
		frame %= m_NrFrames;
	}
	else
	{
		if (frame >= m_NrFrames)
		{
			frame = m_NrFrames - 1;
		}
	}

	return frame;
}

void Animation::DrawAtTime(const Rectf& destRect, float time, bool isMirrored) const
{
	int frame = GetFrameAtTime(time);
	DrawFrame(destRect, frame, isMirrored);
}

float Animation::GetTotalDuration() const
{
	return m_NrFrames * m_FrameSec;
}

bool Animation::IsTimeFinished(float time) const
{
	if (m_IsLooping)
	{
		return false;
	}

	return time >= GetTotalDuration();
}