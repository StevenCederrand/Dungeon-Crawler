#include "AudioEngine.h"

FMOD::System* AudioEngine::m_soundSystem;
std::map<std::string, FMOD::Sound*> AudioEngine::m_sounds;
std::vector<FMOD::Channel*> AudioEngine::m_channels;
void* AudioEngine::m_extraDriverData;


AudioEngine::AudioEngine() {
	if (init() != FMOD_OK) {
		LOG_WARNING("AUDIO ENGINE INIT FAILED");
	}
	else {
		LOG_INFO("AUDIO ENGINE SETUP");
	}
}

AudioEngine::~AudioEngine() {
	
	m_soundSystem->close();
	m_soundSystem->release();
}

FMOD_RESULT AudioEngine::init() {
	FMOD_RESULT res;
	unsigned int version;

	res = FMOD::System_Create(&m_soundSystem);

	if (res != FMOD_OK) {
		LOG_ERROR("FMOD CAN'T SETUP");
		return res;
	}
	res = m_soundSystem->getVersion(&version);

	if (version < FMOD_VERSION) {
		LOG_WARNING("FMOD VERSIONS AREN'T SYNCED");
	}

	m_soundSystem->init(512, FMOD_INIT_NORMAL, m_extraDriverData);
	return res;
}

FMOD::System * AudioEngine::getSoundSystem()
{
	return m_soundSystem;
}

//This works
FMOD_RESULT AudioEngine::loadSound(std::string name, std::string key) {
	if (keyInUse(key)) {
		LOG_WARNING("Sound key already in use");
		return FMOD_RESULT::FMOD_ERR_BADCOMMAND;
	}
	FMOD::Sound* sound;
	FMOD_RESULT res = m_soundSystem->createSound(std::string(SoundPath + name).c_str(), FMOD_DEFAULT, 0, &sound);

	if (res != FMOD_OK) {
		LOG_ERROR("ERROR CREATING SOUND");
		return res;
	}

	m_sounds[key] = sound;
	LOG_INFO("Number of sounds: " + std::to_string(m_sounds.size()));

	return res;
}

FMOD_RESULT AudioEngine::loadSound(std::string name, std::string key, bool looping, bool m3DSound, bool stream) {
	FMOD_RESULT res = FMOD_OK;

	if (keyInUse(key)) {
		LOG_WARNING("Sound key already in use");
		return res;
	}
	
	FMOD_MODE soundMode;
	soundMode |= m3DSound ? FMOD_3D : FMOD_2D;
	soundMode |= looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	soundMode |= stream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
	
	FMOD::Sound* sound = nullptr;
	res = m_soundSystem->createSound(std::string(SoundPath + name).c_str(), soundMode, nullptr, &sound);

	//Continue assuming everything went well
	m_sounds[key] = sound;
	LOG_INFO("Number of sounds: " + std::to_string(m_sounds.size()));
	return res;
}

bool AudioEngine::unloadSound(std::string key) {
	//if the key exists
	if (m_sounds.find(key) != m_sounds.end()) {
		m_sounds[key]->release();
		m_sounds.erase(key);
		LOG_INFO("Number of sounds: " + std::to_string(m_sounds.size()));
	}
	else {
		LOG_WARNING("Key: " + key + "| doesn't exist");
		return false;
	}

	return true;
}

void AudioEngine::update() {
	//Loop through the channels in use and then stop and remove then if they aren't in use
	bool isPlaying;
	m_soundSystem->update();
	for (size_t i = 0; i < m_channels.size(); i++) {
		FMOD::Channel* temp = m_channels.at(i);
		temp->isPlaying(&isPlaying);
		if (!isPlaying) {	
			temp->stop();
			m_channels.erase(m_channels.begin() + i);
		}
	}
}

void AudioEngine::play(std::string key, float volume) {
	if (keyInUse(key)) {
		FMOD_RESULT res;
		FMOD::Channel* channel;
		if (m_sounds.at(key) == nullptr) {
			LOG_ERROR("key not active");
			return;
		}
		if (playingSound(key)) {
			LOG_WARNING("Sound already playing");
			return;
		}
		res = m_soundSystem->playSound(m_sounds.at(key), 0, false, &channel);
		
		if (res != FMOD_OK) {
			LOG_ERROR("ERROR PLAYING SOUND");
			return;
		}

		channel->setVolume(volume);
		m_channels.push_back(channel);
	}
}

bool AudioEngine::keyInUse(std::string key) { 
	if (m_sounds.find(key) != m_sounds.end()) {
		return true;
	}
	return false;
}

bool AudioEngine::playingSound(std::string key) {
	FMOD::Sound* currentSound;
	for (size_t i = 0; i < m_channels.size(); i++) {
		m_channels.at(i)->getCurrentSound(&currentSound);
		if (currentSound == m_sounds[key]) {
			return true;
		}
	}
	return false;
}

