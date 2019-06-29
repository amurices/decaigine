#include "SpriteBatch.h"

namespace Denjine{

SpriteBatch::SpriteBatch() : _vbo(0), _vao(0)
{
}



SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::begin(GlyphSort type /* = GlyphSort::TEXTURE */)
{
	_sortType = type;
	_renderBatches.clear();
	_glyphsToDraw.clear();
}

void SpriteBatch::end()
{
	// First, set up the pointer array. Memory sorting is faster
	_glyphsToSort.resize(_glyphsToDraw.size());
	for (int i = 0; i < _glyphsToDraw.size(); i++)
	{
		_glyphsToSort[i] = &(_glyphsToDraw[i]);
	}
	// Sort glyphs based on texture for the next frame
	sortGlyphs();
	createRenderBatches();
}

void SpriteBatch::init()
{
	createVertexArray();
}

void SpriteBatch::sortGlyphs()
{
	switch (_sortType)
	{
	case GlyphSort::BACK_TO_FRONT:
		std::stable_sort(_glyphsToSort.begin(), _glyphsToSort.end(), cmpBackToFront);
		break;
	case GlyphSort::FRONT_TO_BACK:
		std::stable_sort(_glyphsToSort.begin(), _glyphsToSort.end(), cmpFrontToBack);
		break;
	case GlyphSort::TEXTURE:
		std::stable_sort(_glyphsToSort.begin(), _glyphsToSort.end(), cmpTexture);
		break;
	}
	
}

void SpriteBatch::draw(const glm::vec4& pos, const glm::vec4& texCoords, GLuint tex, float dep, const Color& col, float ang )
{
	// We just add whatever sprite is determined by our parameters to our container of glyphs to be drawn
	_glyphsToDraw.emplace_back(pos, texCoords, tex, dep, col, ang);
}

void SpriteBatch::renderBatch()
{
	// Bind the vertex attribute array before drawing
	glBindVertexArray(_vao);

	for (int i = 0; i < _renderBatches.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
		glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
	}

	// Unbind it
	glBindVertexArray(0);
}

void SpriteBatch::createRenderBatches()
{
	// No batches to create (nothing to render)
	if (_glyphsToDraw.empty())
		return;

	std::vector<Vertex> vertices;
	vertices.resize(_glyphsToDraw.size() * 6);

	int cv = 0;
	int offset = 0;

	_renderBatches.emplace_back(offset, 6, _glyphsToDraw[0].texture); // We do it imperatively with one glyph first*
	vertices[cv++] = _glyphsToDraw[0].upperLeft;
	vertices[cv++] = _glyphsToDraw[0].lowerLeft;
	vertices[cv++] = _glyphsToDraw[0].upperRight;

	vertices[cv++] = _glyphsToDraw[0].lowerRight;
	vertices[cv++] = _glyphsToDraw[0].lowerLeft;
	vertices[cv++] = _glyphsToDraw[0].upperRight;

	offset += 6;
	for (int cg = 1; cg < _glyphsToDraw.size(); cg++)
	{
		// *so we don't bug down here
		if (_glyphsToDraw[cg].texture == _glyphsToDraw[cg - 1].texture) // If we have the same texture, increase size of our RenderBatch
		{
			_renderBatches.back().numVertices += 6;
		}
		else // If there has been a change in texture (meaning we will add no more glyphs to the current renderBatch)
		{
			_renderBatches.emplace_back(offset, 6, _glyphsToDraw[cg].texture);
		}

		// We fill the vertex array with the vertices of each glyph we need to draw
		vertices[cv++] = _glyphsToDraw[cg].upperLeft;
		vertices[cv++] = _glyphsToDraw[cg].lowerLeft;
		vertices[cv++] = _glyphsToDraw[cg].upperRight;
		// 
		vertices[cv++] = _glyphsToDraw[cg].lowerRight;
		vertices[cv++] = _glyphsToDraw[cg].lowerLeft;
		vertices[cv++] = _glyphsToDraw[cg].upperRight;

		offset += 6;
	}


	// Bind the vbo and upload the vertex data to the graphics card,
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
	// then unbind the vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void SpriteBatch::createVertexArray()
{
	// If the vao has not already been created, we do so. We don't want to create multiple vertex arrays
	if (_vao == 0)
		glGenVertexArrays(1, &_vao);

	glBindVertexArray(_vao);

	// Same for the vbo
	if (_vbo == 0)
		glGenBuffers(1, &_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	//Tell opengl that we want to use the first
	//attribute array. We only need one array right
	//now since we are only using position.
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


	//Point opengl to the data in our VBO. Position:
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	// Color:
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	// texUV:
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texuv)));

	// unbind
	glBindVertexArray(0);

}

bool SpriteBatch::cmpFrontToBack(Glyph *a, Glyph *b)
{
	return (a->depth < b->depth);
}
bool SpriteBatch::cmpBackToFront(Glyph *a, Glyph *b)
{
	return (a->depth > b->depth);
}
bool SpriteBatch::cmpTexture(Glyph *a, Glyph *b)
{
	return (a->texture < b->texture);
} 
}