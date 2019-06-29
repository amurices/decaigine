#pragma once
#include "Headerz.h"
#include "Vertex.h"

namespace Denjine{

	enum	EntityFlags{ DEFAULT = 0x0, NOT_VISIBLE = 0x1, DYNAMIC = 0x2, UNCOLLIDABLE = 0x4, NO_GRAVITY = 0x8 };
	enum	ShapeType{ CIRCLE, POLYGON, EDGE, CHAIN, LOOPED_CHAIN };
			

class Entity
{
public:
	Entity();
	~Entity();

	unsigned int	_flags = 0x0; // Entities are visible, static and collidable by default
	
	// physical body properties
	b2World*					_entityWorld;
	b2Body*						_entityBody;
	glm::vec2					_entityDims;

	// visual body properties (to add to the spriteBatch)
	glm::vec4			_drawPosDim;
	glm::vec4			_drawUvDim;
	float				_drawAngle;
	GLuint				_texID;
	float				_depth;
	Color				_color;

	//  -------------- Graphics functions
	void setTexData(const glm::vec4 &drawUVD, GLuint tex);
	void update();		// equate the physical to the visual

	//  -------------- Physics functions
	// set the position and type of physical body
	void setEntityBody(unsigned int flags, glm::vec2 pos);
	// Create a circle and attach it to the body
	void createEntityCircleFixture(float radius, float density, float friction);
	// Create a shape and attach it to the body
	void createEntityFixture(ShapeType type, const b2Vec2* vertices, int numVertices, float density, float friction);


	


};

}
