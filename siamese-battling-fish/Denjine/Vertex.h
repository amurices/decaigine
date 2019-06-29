#pragma once

#include <GL/glew.h>

namespace Denjine{


	struct Position { //Position is the type
		float x;
		float y;
	}; 

	//4 bytes for r g b a color.
	struct Color {
		// Default constructor
		Color() : r(0), g(0), b(0), a(0) {
		}
		// COMPLEX constructor
		Color(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
		{
			r = red; g = green; b = blue; a = alpha;
		}
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	// texture coordinates
	struct UV{
		float u;
		float v;
	};

	// Helper header to process vertex data
	struct Vertex {
		struct Position position;

		//4 bytes for r g b a color.
		struct Color color;

		// texture coordinates
		struct UV texuv;

		void setColor(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
		{
			color.r = red;
			color.g = green;
			color.b = blue;
			color.a = alpha;
		}

		void setPosition(float ex, float uai)
		{
			position.x = ex;
			position.y = uai;
		}

		void setUV(float yoo, float vee)
		{
			texuv.u = yoo;
			texuv.v = vee;
		}
	};
}