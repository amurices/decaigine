#include "ResourceManager.h"

namespace Denjine{

	TextureResources	ResourceManager::_texResources;
	AudioResources		ResourceManager::_audResources;

	GLuint ResourceManager::getTexture(std::string texPath)
	{
		return _texResources.getTexture(texPath);
	}

	void ResourceManager::getAudio(std::string audPath, FMOD::Sound** audio, FMOD::System *system, PlaybackType t)
	{
		_audResources.getAudio(audPath, audio, system, t);
	}

	void ResourceManager::unloadResources(FMOD::System *system)
	{
		_texResources.unloadTextures();
		_audResources.unloadAudio(system);
	}
}