#include "TextureResources.h"

namespace Denjine{

	TextureResources::TextureResources()
	{
	}


	TextureResources::~TextureResources()
	{
	}

	GLuint TextureResources::getTexture(std::string texPath)
	{
		auto texturePointer = _textureContainer.find(texPath);
		if (texturePointer == _textureContainer.end())		// So this is a new texture
		{
			GLuint newTex = IOManager::loadTextureFromFile(texPath); // If we got to the end of the existing textures, load the new one
			_textureContainer.insert(make_pair(texPath, newTex));
			std::cout << "Adding a new texture!" << std::endl;
			return newTex;
		}
		std::cout << "Found a repeated texture; not adding it!" << std::endl;

		return texturePointer->second;
	}

	void TextureResources::unloadTextures()
	{
		std::map<std::string, GLuint>::iterator			textureIterator;
		for (textureIterator = _textureContainer.begin(); textureIterator != _textureContainer.end(); textureIterator++)
		{
			glDeleteTextures(1, &(*textureIterator).second);
		}

	}
}