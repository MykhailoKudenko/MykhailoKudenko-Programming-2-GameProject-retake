#include "pch.h"
#include "Camera.h"
#include <cmath>

Camera::Camera(float screenWidth, float screenHeight)
	: m_ScreenWidth{ screenWidth }
	, m_ScreenHeight{ screenHeight }
{
}

void Camera::Aim(float levelWidth, float levelHeight, float xNegativeSpace, float yNegativeSpace, const Vector2f& trackCenter, float zoom) const
{
	float viewWidth = m_ScreenWidth / zoom;
	float viewHeight = m_ScreenHeight / zoom;

	float cameraLeft = trackCenter.x - viewWidth / 2.0f;
	float cameraBottom = trackCenter.y - viewHeight / 2.0f;

	if (cameraLeft < -xNegativeSpace)
	{
		cameraLeft = -xNegativeSpace;

	}
	if (cameraLeft > levelWidth - viewWidth)
	{
		cameraLeft = levelWidth - viewWidth;

	}
	if (cameraBottom < -yNegativeSpace)
	{
		cameraBottom = -yNegativeSpace;

	}
	if (cameraBottom > levelHeight - viewHeight)
	{
		cameraBottom = levelHeight - viewHeight;

	}
	if (levelWidth < viewWidth)
	{
		cameraLeft = -xNegativeSpace;

	}
	if (levelHeight < viewHeight)
	{
		cameraBottom = -yNegativeSpace;

	}

	glPushMatrix();
	glScalef(zoom, zoom, 1.0f);
	glTranslatef(-cameraLeft, -cameraBottom, 0.0f);
}
void Camera::Reset() const
{
	glPopMatrix();
}