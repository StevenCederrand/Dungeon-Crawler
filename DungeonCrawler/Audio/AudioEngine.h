#ifndef AUDIOENGINE_h
#define AUDIOENGINE_h

#include <FMOD/fmod.hpp>
#include <FMOD/fmod.h>
#include "../System/Log.h"
#include "../Globals/Paths.h"
#include <map>
#include <vector>
#include "../Parser/Parser.h"
#include <fstream>
#include <string>
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
	static bool unloadSSO(std::string ssoName);
	static void update();
	//Play a sound
	static FMOD::Channel* play(std::string key, float volume);
	//play the entire sound without being able to replay it
	static void playOnce(std::string key, float volume);
	
	//Stop playing the specific sound
	static void stop(std::string key);
private:
	static FMOD_RESULT init();
	//Check to see if a key in the hashmap is in use
	static bool keyInUse(std::string key);
	static bool playingSound(std::string key);
	static FMOD::Channel* getChannel(const char* key);
	
	//This is to avoid playing multiple instances of a related sound
	//I.e we shouldn't overlap walk1 with walk2, instead we should wait for walk1 to complete
	static bool playingRelatedSound(std::string key); 

private:

	//Sound effect containers
	static FMOD::System *m_soundSystem;
	static std::map<std::string, FMOD::Sound*> m_sounds;	
	static std::vector<FMOD::Channel*> m_channels;

	static void* m_extraDriverData;
	static std::vector<std::string> keysInUse;
};



#endif