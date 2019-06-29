#include "AudioResources.h"

namespace Denjine{

AudioResources::AudioResources()
{
}


AudioResources::~AudioResources()
{
}

void AudioResources::getAudio(std::string audPath, FMOD::Sound** audio, FMOD::System* system, PlaybackType t)
{
	auto audioPointer = _audioContainer.find(audPath);
	FMOD_RESULT resultCode = FMOD_OK;
	if (audioPointer == _audioContainer.end())		// So this is a new audio file
	{
		if (t == PlaybackType::SOUND){
			resultCode = system->createSound(audPath.c_str(), FMOD_DEFAULT, 0, audio); // load the sound into the memory address provided
		}

		else if (t == PlaybackType::STREAM){
			resultCode = system->createStream(audPath.c_str(), FMOD_DEFAULT, 0, audio); // load the stream into the memory address provided
		}

		if (resultCode != FMOD_OK) // Error checking, as always
		{
			std::string errorString = "FMOD error! Error code: " + std::to_string(resultCode);
			errorString += ", Description: " + std::string(FMOD_ErrorString(resultCode));
			fatalError(errorString);
		}
		_audioContainer.insert(make_pair(audPath, audio));

		std::cout << "Adding a new audio file!" << std::endl;
		return;
	}
	std::cout << "Found a repeated audio file; not adding it!" << std::endl;
}

void AudioResources::unloadAudio(FMOD::System* system)
{
	std::map<std::string, FMOD::Sound**>::iterator			audioIterator;
	for (audioIterator = _audioContainer.begin(); audioIterator != _audioContainer.end(); audioIterator++)
	{
		(*(*audioIterator).second)->release();
	}
	system->release();

}

}