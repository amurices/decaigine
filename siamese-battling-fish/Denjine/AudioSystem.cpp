#include "AudioSystem.h"

namespace Denjine{

AudioSystem::AudioSystem()
{
}


AudioSystem::~AudioSystem()
{
}



void AudioSystem::loadBGM(std::string songPath)
{
	if (_bgmObject0 != 0)
	{
		ResourceManager::getAudio(songPath, &(_bgmObject0), _system, Denjine::PlaybackType::STREAM);
	}

	else if (_bgmObject1 != 0)
	{
		ResourceManager::getAudio(songPath, &(_bgmObject1), _system, Denjine::PlaybackType::STREAM);
	}

	else
	{
		return; // both bgm objects are taken
	}
}

void AudioSystem::playAudio(FMOD::Sound *audio, bool paused, FMOD::Channel **channelHandle)
{
	FMOD_RESULT resultCode;
	resultCode = _system->playSound(audio, 0, paused, channelHandle);
	if (resultCode != FMOD_OK) // Error checking, as always
	{
		std::string errorString = "FMOD error! Error code: " + std::to_string(resultCode);
		errorString += ", Description: " + std::string(FMOD_ErrorString(resultCode));
		fatalError(errorString);
	}
}


void AudioSystem::init()
{
	FMOD_RESULT			resultCode;
	FMOD_SPEAKERMODE	speakerMode;
	int					speakerModeChannels;
	unsigned int		fmodVersion;
	int					numDrivers;

	// Address of the pointer (???) is given to set up the playback environment and error check
	resultCode = FMOD::System_Create(&_system);
	if (resultCode != FMOD_OK) // Error checking, as always
	{
		std::string errorString = "FMOD error! Error code: " + std::to_string(resultCode);
		errorString += ", Description: " + std::string(FMOD_ErrorString(resultCode));
		fatalError(errorString);
	}

	// find out how many sound cards there are and error check
	resultCode = _system->getNumDrivers(&numDrivers);
	if (resultCode != FMOD_OK) // Error checking, as always
	{
		std::string errorString = "FMOD error! Error code: " + std::to_string(resultCode);
		errorString += ", Description: " + std::string(FMOD_ErrorString(resultCode));
		fatalError(errorString);
	}

	// If there are 0 sound cards
	if (!numDrivers)
	{
		resultCode = _system->setOutput(FMOD_OUTPUTTYPE_NOSOUND); // set the outputType to "no sound" and error check
		if (resultCode != FMOD_OK) // Error checking, as always
		{
			std::string errorString = "FMOD error! Error code: " + std::to_string(resultCode);
			errorString += ", Description: " + std::string(FMOD_ErrorString(resultCode));
			fatalError(errorString);
		}
	}

	else
	{
		// get the speaker mode of this device's driver
		_system->getDriverInfo(0, 0, 0, 0, 0, &speakerMode, &speakerModeChannels);
		// set the speaker mode of our system to be the same as of this device's
	//	_system->setSoftwareFormat()
	}

	// Finally we'll initialize FMOD
	resultCode = _system->init(100, FMOD_INIT_NORMAL, 0);
	if (resultCode != FMOD_OK) // Error checking, as always
	{
		std::string errorString = "FMOD error! Error code: " + std::to_string(resultCode);
		errorString += ", Description: " + std::string(FMOD_ErrorString(resultCode));
		fatalError(errorString);
	}

	_bgmObject0 = 0;	// If we don't get any other errors,
	_bgmObject1 = 0;	// just init these to 0 so we know they're not taken.
}
}