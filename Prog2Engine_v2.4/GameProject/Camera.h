#pragma once

class Camera final
{
public:
	explicit Camera(const Vector2f& windowSize);

	void Aim(float levelWidth, float levelHeight, float xNegativeSpace, float yNegativeSpace, const Vector2f& trackCenter, float zoom) const;
	void Reset() const;

private:
	float m_ScreenWidth;
	float m_ScreenHeight;
};
