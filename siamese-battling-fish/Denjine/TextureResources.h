#pragma once
#include "Headerz.h"
#include "IOManager.h"

namespace Denjine{

// This is a class that will store every texture resource. ResourceManager will manage these resources,
// but since we won't have instances of ResourceManager, we must have instances of containers that actually have the assets in them.
class TextureResources
{
public:
	TextureResources();
	~TextureResources();

	// Look for a texture resource and return a "pointer" (GLuint) to it
	GLuint getTexture(std::string texPath);
	void unloadTextures();
private:
	// Store GLuints; they're kind of pointers anyway, as each one identifies a different texture
	std::map<std::string, GLuint> _textureContainer;
};

}