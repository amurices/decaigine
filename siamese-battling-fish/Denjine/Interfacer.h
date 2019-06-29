#pragma once
// The interfacing subsystem needs common headers, since it's wrapped around SDL functionality
#include "Headerz.h"

namespace Denjine{


	// This class will handle input. It's mostly a wrapper around SDL events and input functionality.
	class Interfacer
	{
	public:
		Interfacer();
		~Interfacer();

		// Has to run at the end of every frame to update the previous keyMap
		void updateInterfacer();

		// Tell the interfacer that we've pressed a key
		void pressKey(unsigned int keyID);

		// tell it we've released a key
		void releaseKey(unsigned int keyID);

		// check to see if a key is pressed
		bool isKeyPressed(unsigned int keyID);

		// check to see if this is the first time this key has been pressed since last frame
		bool isKeyTriggered(unsigned int keyID);

		void getInput(); // The master function of all things user input-related, it gives us everything that happens
	private:
		// Keeps bool values for each key
		std::unordered_map<unsigned int, bool> _currentKeyMap;
		// Keeps bool values of 
		std::unordered_map<unsigned int, bool> _previousKeyMap;

	};
}
