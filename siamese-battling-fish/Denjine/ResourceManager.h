#pragma once
#include "TextureResources.h"
#include "AudioResources.h"
#include "Errors.h"

namespace Denjine{

	class ResourceManager
	{
	public:
		static GLuint		getTexture(std::string texPath);
		static void			getAudio(std::string audPath, FMOD::Sound** audio, FMOD::System *system, PlaybackType t);
		static void			unloadResources(FMOD::System *system);

	private:
		static TextureResources _texResources;
		static AudioResources	_audResources;

	};

}