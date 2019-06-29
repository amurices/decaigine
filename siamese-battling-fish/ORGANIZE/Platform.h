#pragma once
#include <Denjine\Headerz.h>
#include <Denjine\Entity.h>

// Platforms are essentially boxes. we'll set draw data and all that good stuff.
// All this class does is implement static, physical squares that act as the game's
// walls and floors. a wrapper around a certain standard parametrization of the Entity class.

class Platform
{
public:
	Platform();
	Platform::Platform(b2World* gameWorld, const glm::vec2 &pos, const glm::vec2 &dims, const glm::vec4 &drawData, GLint texture); // constructor gives data

	~Platform();

	b2World*			_gameWorld;
	Denjine::Entity		_entity; // we want our entities in our global stack

	void init(); // init gives data from handle to entity
	void update();
	

	glm::vec2	_pos;
	glm::vec2	_dims;
	glm::vec4*	_drawPosDims; // we won't account for color. we want our game objects to be textured
	glm::vec4	_drawUvDims;
	GLint		_texture;
	
};

