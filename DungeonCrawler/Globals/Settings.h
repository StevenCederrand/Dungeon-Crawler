#ifndef SETTINGS_h
#define SETTINGS_h

class Application;

class Settings {
public:

	static int getScreenWidth();
	static int getScreenHeight();
	static int getMaxLights();

private:
	friend class Application; // So Application can set the values but no other class can
	static void setResolution(const int width, const int height);
	static void setMaxLights(const int maxLights);

	static int m_screenWidth;
	static int m_screenHeigth;
	static int m_maxLights;
};

#endif