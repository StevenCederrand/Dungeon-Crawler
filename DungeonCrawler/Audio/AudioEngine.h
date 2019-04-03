#ifndef AUDIOENGINE_h
#define AUDIOENGINE_h

#include <FMOD/fmod.hpp>
#include <FMOD/fmod.h>
#include "../System/Log.h"
#include "../Globals/Paths.h"
#include <map>
#include <vector>
#include "../Parser/Parser.h"

class AudioEngine {
public:
	AudioEngine();
	~AudioEngine();
	
	static FMOD::System* getSoundSystem();
	//Used for loading setlists. 
	static FMOD_RESULT loadSSO(std::string name);
	static FMOD_RESULT loadSound(std::string name, std::string key);
	static FMOD_RESULT loadSound(std::string name, std::string key, bool looping, bool m3DSound, bool stream);
	//Remove a sound from the map
	static bool unloadSound(std::string key);
	static void update();
	//Play a sound
	static void play(std::string key, float volume);

private:
	static FMOD_RESULT init();
	//Check to see if a key in the hashmap is in use
	static bool keyInUse(std::string key);
	static bool playingSound(std::string key);
	//Sound effect containers
	static FMOD::System *m_soundSystem;
	static std::map<std::string, FMOD::Sound*> m_sounds;	
	static std::vector<FMOD::Channel*> m_channels;
	static void* m_extraDriverData;
};



#endif