#pragma once
#include "Headerz.h"
#include "Errors.h"
#include "ResourceManager.h"

namespace Denjine{

// Mostly a wrapper for the audio system
class AudioSystem
{
public:
	AudioSystem();
	~AudioSystem();

	// Set up the environment for audio playback
	void init();

	// Two channel handles for background music
	FMOD::Channel*	_bgmChannel0;
	FMOD::Channel*	_bgmChannel1;

	// Two memory places for background music
	FMOD::Sound*	_bgmObject0;
	FMOD::Sound*	_bgmObject1;

	void loadBGM(std::string songPath);

	// Wrapper around FMOD::System->playSound; by default, we'll play sounds on free channels
	void playAudio(FMOD::Sound *audio, bool paused, FMOD::Channel **channelHandle);

	// governor of all things
	FMOD::System* _system; 


};

}
