#ifndef AUDIOENGINE_h
#define AUDIOENGINE_h

#include <FMOD/fmod.hpp>
#include <FMOD/fmod.h>
#include "../System/Log.h"
#include "../Globals/Paths.h"
#include <map>
#include <vector>

class AudioEngine {

public:
	AudioEngine();
	~AudioEngine();

	FMOD::System* getSoundSystem();
	//loadSound(Name, is the sound looping?, is it a 3D sound?, should we stream it?) : Name assumes that the sound is in the audio folder, include extension at the end of the name
	FMOD_RESULT loadSound(std::string name, std::string key);
	FMOD_RESULT loadSound(std::string name, std::string key, bool looping, bool m3DSound, bool stream);
	//Remove a sound from the map
	bool unloadSound(std::string key);
	void update();
	//Play a sound
	void play(std::string key, float volume);
private:
	//Check to see if a key in the hashmap is in use
	bool keyInUse(std::string key);
	
	FMOD::System *m_soundSystem;
	std::map<std::string, FMOD::Sound*> m_sounds;
	
	std::vector<FMOD::Channel*> channels;

	void* m_extraDriverData;
};



#endif