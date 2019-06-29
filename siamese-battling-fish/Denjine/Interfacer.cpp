#include "Interfacer.h"
namespace Denjine{


	Interfacer::Interfacer()
	{
	}


	Interfacer::~Interfacer()
	{
	}

	void Interfacer::updateInterfacer()
	{
		_previousKeyMap = _currentKeyMap;
	}

	void Interfacer::pressKey(unsigned int keyID)
	{
		// We only set a key to be pressed in our map
		_currentKeyMap[keyID] = true;
	}

	void Interfacer::releaseKey(unsigned int keyID)
	{
		// We only set a key to be pressed in our map
		_currentKeyMap[keyID] = false;
	}

	bool Interfacer::isKeyPressed(unsigned int keyID)
	{
		// See if keyID has ever even been created in the map
		auto it = _currentKeyMap.find(keyID);
		if (it != _currentKeyMap.end())		// If we find a correspondence in the map,
		{
			return _currentKeyMap[keyID];		// we return it
		}
		else
			return false;				// otherwise, the key has never even been pressed, so we say false
	}

	bool Interfacer::isKeyTriggered(unsigned int keyID)
	{
		auto it = _previousKeyMap.find(keyID);
		if (it != _previousKeyMap.end())
		{
			if (_previousKeyMap[keyID])
				return false;
			else if (_currentKeyMap[keyID])
				return true;
		}
	}

	void Interfacer::getInput()
	{
		SDL_Event e; // Variable to poll what happens, user-wise

		while (SDL_PollEvent(&e))	// We just want the interfacer to get our input. How we handle it is up to the Game class.
			// So we'll do that by 
		{

		}
	}
}