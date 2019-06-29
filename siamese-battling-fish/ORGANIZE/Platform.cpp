#include "Platform.h"


Platform::Platform()
{
}

Platform::Platform(b2World* gameWorld, const glm::vec2 &pos, const glm::vec2 &dims, const glm::vec4 &drawData, GLint texture)
{
	_gameWorld = gameWorld;
	_pos = pos;
	_dims = dims;
	_drawUvDims = drawData;
	_texture = texture;
}

Platform::~Platform()
{
}

void Platform::init()
{
	_entity._entityWorld = _gameWorld;
	_entity.setEntityBody(Denjine::EntityFlags::DEFAULT, _pos);

	b2Vec2 BERtices[4];
	BERtices[0].Set(-(_dims.x * 0.5), -(_dims.y * 0.5));
	BERtices[1].Set(-(_dims.x * 0.5), _dims.y * 0.5);
	BERtices[2].Set(_dims.x * 0.5, -(_dims.y * 0.5));
	BERtices[3].Set(_dims.x * 0.5, _dims.y * 0.5);


	_entity.createEntityFixture(Denjine::ShapeType::POLYGON, BERtices, 4, 1.0f, 0.1f);

	_entity.setTexData(_drawUvDims, _texture);
}
void Platform::update()
{
	_entity.update();
	_drawPosDims = &(_entity._drawPosDim);
}