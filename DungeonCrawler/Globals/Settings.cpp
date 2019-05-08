#include "Settings.h"

int Settings::m_screenWidth;
int Settings::m_screenHeigth;
int Settings::m_maxLights;

int Settings::getScreenWidth()
{
	return m_screenWidth;
}

int Settings::getScreenHeight()
{
	return m_screenHeigth;
}

int Settings::getMaxLights()
{
	return m_maxLights;
}

void Settings::setResolution(const int width, const int height)
{
	m_screenWidth = width;
	m_screenHeigth = height;
}

void Settings::setMaxLights(const int maxLights)
{
	m_maxLights = maxLights;
}
