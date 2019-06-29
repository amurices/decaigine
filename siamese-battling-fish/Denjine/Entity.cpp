#include "Entity.h"


namespace Denjine{

Entity::Entity()
{
	_entityDims.x = 0;
	_entityDims.y = 0;
}


Entity::~Entity()
{
}

void Entity::update()
{
	if (!(_flags & EntityFlags::NOT_VISIBLE)) // if entity is not not visible, upload the draw data
	{
		_drawPosDim.x = _entityBody->GetPosition().x - _entityDims.x;
		_drawPosDim.y = _entityBody->GetPosition().y - _entityDims.y;
		_drawPosDim.z = _entityDims.x * 2;
		_drawPosDim.w = _entityDims.y * 2;
		_drawAngle = _entityBody->GetAngle();
	}
//	std::cout << "drawPos = " << _drawPosDim.x << ", " << _drawPosDim.y << ", " << _drawPosDim.z << ", " << _drawPosDim.w << std::endl;
}


void Entity::setTexData(const glm::vec4& drawUVD, GLuint tex)
{
	_drawUvDim = drawUVD;
	_texID = tex;
}


void Entity::setEntityBody(unsigned int flags, glm::vec2 pos)
{
	b2BodyDef entityBodyDef;

	entityBodyDef.position.Set(pos.x, pos.y);
	if (flags & EntityFlags::DYNAMIC)
	{
		entityBodyDef.type = b2_dynamicBody;
	}
	if (flags & EntityFlags::NO_GRAVITY)
	{
		entityBodyDef.gravityScale = 0.0f;
	}
	_entityBody = _entityWorld->CreateBody(&entityBodyDef);
	

}

void Entity::createEntityCircleFixture(float radius, float density, float friction)
{
	b2AABB			fixtureAABB;
	b2FixtureDef	fixtureDef;
	b2CircleShape	fixtureShape;
	fixtureShape.m_radius	= radius;
	fixtureDef.density		= density;
	fixtureDef.friction		= friction;
	b2Fixture* myFixture = _entityBody->CreateFixture(&fixtureDef);
	fixtureAABB = myFixture->GetAABB(0);
	_entityDims = glm::vec2(fixtureAABB.GetExtents().x, fixtureAABB.GetExtents().y);

}

void Entity::createEntityFixture(ShapeType type, const b2Vec2* vertices, int numVertices, float density, float friction)
{
	b2FixtureDef	fixtureDef;
	if (type == ShapeType::POLYGON)
	{
		b2AABB			fixtureAABB;
		b2PolygonShape	fixtureShape;
		fixtureShape.Set(vertices, numVertices);
		fixtureDef.shape = &fixtureShape;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		b2Fixture* myFixture = _entityBody->CreateFixture(&fixtureDef);
		if (_entityDims.x == 0 && _entityDims.y == 0) // if so, this is the first fixture of a shape to the body
		{
			fixtureAABB = myFixture->GetAABB(0);
			_entityDims = glm::vec2(fixtureAABB.GetExtents().x, fixtureAABB.GetExtents().y);
		}
		else // if not, we iterate through every fixture and find the new body dimensions
		{
			b2Vec2 lb, ub;
			myFixture = _entityBody->GetFixtureList();
			lb = myFixture->GetAABB(0).lowerBound;
			ub = myFixture->GetAABB(0).upperBound;

			for (; myFixture != NULL; myFixture = myFixture->GetNext())
			{
				fixtureAABB = myFixture->GetAABB(0);
				if (fixtureAABB.lowerBound.x < lb.x)
					lb.x = fixtureAABB.lowerBound.x;
				if (fixtureAABB.lowerBound.y < lb.y)
					lb.y = fixtureAABB.lowerBound.y;
				if (fixtureAABB.upperBound.x > ub.x)
					ub.x = fixtureAABB.upperBound.x;
				if (fixtureAABB.upperBound.y > ub.y)
					ub.y = fixtureAABB.upperBound.y;
			}
			ub = ub - lb;
			_entityDims.x = ub.x / 2;
			_entityDims.y = ub.y / 2;
		}

	}
	else if (type == ShapeType::CHAIN)
	{
		b2AABB			fixtureAABB;
		b2ChainShape fixtureShape;
		fixtureShape.CreateChain(vertices, numVertices);
		fixtureDef.shape = &fixtureShape;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		b2Fixture* myFixture = _entityBody->CreateFixture(&fixtureDef);
		fixtureAABB = myFixture->GetAABB(0);
		_entityDims = glm::vec2(fixtureAABB.GetExtents().x, fixtureAABB.GetExtents().y);

	}
	else if (type == ShapeType::EDGE)
	{
		b2AABB			fixtureAABB;
		b2EdgeShape fixtureShape;

		fixtureShape.Set(vertices[0], vertices[1]);
		fixtureDef.shape = &fixtureShape;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		b2Fixture* myFixture = _entityBody->CreateFixture(&fixtureDef);
		fixtureAABB = myFixture->GetAABB(0);
		_entityDims = glm::vec2(fixtureAABB.GetExtents().x, fixtureAABB.GetExtents().y);

	}
	else if (type == ShapeType::LOOPED_CHAIN)
	{
		b2AABB			fixtureAABB;
		b2ChainShape fixtureShape;

		fixtureShape.CreateLoop(vertices, numVertices);
		fixtureDef.shape = &fixtureShape;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		b2Fixture* myFixture = _entityBody->CreateFixture(&fixtureDef);
		fixtureAABB = myFixture->GetAABB(0);
		_entityDims = glm::vec2(fixtureAABB.GetExtents().x, fixtureAABB.GetExtents().y);

	}

	std::cout << "Fixture bounding box dimensions: " << _entityDims.x << ", " << _entityDims.y << std::endl;

}

}