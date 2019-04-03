#include "AudioEngine.h"

AudioEngine::AudioEngine() {
	FMOD_RESULT res;
	unsigned int version;

	res = FMOD::System_Create(&this->m_soundSystem);

	if (res != FMOD_OK) {
		LOG_ERROR("FMOD CAN'T SETUP");
	}
	res = m_soundSystem->getVersion(&version);
	if (version < FMOD_VERSION) {
		LOG_WARNING("FMOD VERSIONS AREN'T SYNCED");
	}

	m_soundSystem->init(512, FMOD_INIT_NORMAL, m_extraDriverData);
}

AudioEngine::~AudioEngine() {
	
	this->m_soundSystem->close();
	this->m_soundSystem->release();
}

FMOD::System * AudioEngine::getSoundSystem()
{
	return this->m_soundSystem;
}

//This works
FMOD_RESULT AudioEngine::loadSound(std::string name, std::string key) {
	if (this->keyInUse(key)) {
		LOG_WARNING("Sound key already in use");
		return FMOD_RESULT::FMOD_ERR_BADCOMMAND;
	}
	FMOD::Sound* sound;
	FMOD_RESULT res = this->m_soundSystem->createSound(std::string(SoundPath + name).c_str(), FMOD_DEFAULT, 0, &sound);

	if (res != FMOD_OK) {
		LOG_ERROR("ERROR CREATING SOUND");
		return res;
	}

	this->m_sounds[key] = sound;
	LOG_INFO("Number of sounds: " + std::to_string(this->m_sounds.size()));

	return res;
}

FMOD_RESULT AudioEngine::loadSound(std::string name, std::string key, bool looping, bool m3DSound, bool stream) {
	FMOD_RESULT res = FMOD_OK;

	if (this->keyInUse(key)) {
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
	this->m_sounds[key] = sound;
	LOG_INFO("Number of sounds: " + std::to_string(this->m_sounds.size()));
	return res;
}

bool AudioEngine::unloadSound(std::string key) {
	//if the key exists
	if (this->m_sounds.find(key) != this->m_sounds.end()) {
		this->m_sounds[key]->release();
		this->m_sounds.erase(key);
		LOG_INFO("Number of sounds: " + std::to_string(this->m_sounds.size()));
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
	this->m_soundSystem->update();
	for (size_t i = 0; i < this->channels.size(); i++) {
		FMOD::Channel* temp = channels.at(i);
		temp->isPlaying(&isPlaying);
		if (!isPlaying) {	
			temp->stop();
			channels.erase(channels.begin() + i);
		}
	}
}

void AudioEngine::play(std::string key, float volume) {
	if (this->keyInUse(key)) {
		FMOD_RESULT res;
		FMOD::Channel* channel;
		if (m_sounds.at(key) == nullptr) {
			LOG_ERROR("key not active");
			return;
		}
		
		res = this->m_soundSystem->playSound(this->m_sounds.at(key), 0, false, &channel);
		
		if (res != FMOD_OK) {
			LOG_ERROR("ERROR PLAYING SOUND");
			return;
		}

		channel->setVolume(volume);
		this->channels.push_back(channel);
	}
}

bool AudioEngine::keyInUse(std::string key) { 
	if (this->m_sounds.find(key) != this->m_sounds.end()) {
		return true;
	}
	return false;
}