#pragma once

#include "Headerz.h"
#include "Vertex.h"

namespace Denjine{

enum class GlyphSort{
	NONE,
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	TEXTURE
};

struct Glyph
{
	Glyph();
	Glyph(const glm::vec4& pos, const glm::vec4& texCoords, GLuint tex, float dep, const Color& col, float angle)
	{
		texture = tex;
		depth = dep;
		if (angle != 0)
		{
			glm::vec2 halfDims(pos.z / 2.0f, pos.w / 2.0f);

			// Get points centered at origin
			glm::vec2 upperLeftOffset(-halfDims.x, halfDims.y);
			glm::vec2 lowerLeftOffset(-halfDims.x, -halfDims.y);
			glm::vec2 lowerRightOffset(halfDims.x, -halfDims.y);
			glm::vec2 upperRightOffset(halfDims.x, halfDims.y);

			// Rotate the points
			upperLeftOffset = rotVertex(upperLeftOffset, angle) + halfDims;
			lowerLeftOffset = rotVertex(lowerLeftOffset, angle) + halfDims;
			lowerRightOffset = rotVertex(lowerRightOffset, angle) + halfDims;
			upperRightOffset = rotVertex(upperRightOffset, angle) + halfDims;

			upperLeft.setColor(col.r, col.g, col.b, col.a);
			upperLeft.setPosition(pos.x + upperLeftOffset.x, pos.y + upperLeftOffset.y);
			upperLeft.setUV(texCoords.x, texCoords.y + texCoords.w);

			lowerLeft.setColor(col.r, col.g, col.b, col.a);
			lowerLeft.setPosition(pos.x + lowerLeftOffset.x, pos.y + lowerLeftOffset.y);
			lowerLeft.setUV(texCoords.x, texCoords.y);

			upperRight.setColor(col.r, col.g, col.b, col.a);
			upperRight.setPosition(pos.x + upperRightOffset.x, pos.y + upperRightOffset.y);
			upperRight.setUV(texCoords.x + texCoords.z, texCoords.y + texCoords.w);

			lowerRight.setColor(col.r, col.g, col.b, col.a);
			lowerRight.setPosition(pos.x + lowerRightOffset.x, pos.y + lowerRightOffset.y);
			lowerRight.setUV(texCoords.x + texCoords.z, texCoords.y);
		}

		else
		{
			upperLeft.setColor(col.r, col.g, col.b, col.a);
			upperLeft.setPosition(pos.x, pos.y + pos.w);
			upperLeft.setUV(texCoords.x, texCoords.y + texCoords.w);

			lowerLeft.setColor(col.r, col.g, col.b, col.a);
			lowerLeft.setPosition(pos.x, pos.y);
			lowerLeft.setUV(texCoords.x, texCoords.y);

			upperRight.setColor(col.r, col.g, col.b, col.a);
			upperRight.setPosition(pos.x + pos.z, pos.y + pos.w);
			upperRight.setUV(texCoords.x + texCoords.z, texCoords.y + texCoords.w);

			lowerRight.setColor(col.r, col.g, col.b, col.a);
			lowerRight.setPosition(pos.x + pos.z, pos.y);
			lowerRight.setUV(texCoords.x + texCoords.z, texCoords.y);
		}
	}

	glm::vec2 rotVertex(const glm::vec2& pos, float angle) {
		
		return (glm::vec2(pos.x * cos(angle) - pos.y * sin(angle), pos.x * sin(angle) + pos.y * cos(angle)));
	}

	GLuint texture;
	float depth;

	Vertex upperRight;
	Vertex lowerRight;
	Vertex upperLeft;
	Vertex lowerLeft;

};



class RenderBatch
{
public:
	inline RenderBatch(GLuint ofs, GLuint nV, GLuint tex){
		offset = ofs; numVertices = nV; texture = tex;
	}
	GLuint offset;
	GLuint numVertices;
	GLuint texture;
};

class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();

	// Set everything up (create our vertex array)
	void init();

	// clear the renderbatch and glyph arrays
	void begin(GlyphSort sortType = GlyphSort::TEXTURE);
	void end();

	// Set up our renderbatches to be drawn. Each vec4 contains, in their last two parameters, the dimensions of the sprite in question
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color, float ang);

	// 
	void renderBatch();

private:
	// Create one batch of glyphs for every texture
	void createRenderBatches();
	// Set up our drawing environment for the whole batch
	void createVertexArray();
	void sortGlyphs();
	
	// Comparators for each type of glyph sorting
	static bool cmpFrontToBack(Glyph* a, Glyph* b);
	static bool cmpBackToFront(Glyph* a, Glyph* b);
	static bool cmpTexture(Glyph* a, Glyph* b);

	GLuint _vbo;
	GLuint _vao;

	GlyphSort _sortType;

	std::vector<Glyph*> _glyphsToSort;
	std::vector<Glyph> _glyphsToDraw;
	std::vector<RenderBatch> _renderBatches;
};

}