#pragma once
#include "Headerz.h"
#include <GLM\gtc\matrix_transform.hpp>
namespace Denjine{

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();
		void initCamera(int _screenWidth, int _screenHeight);
		void updateCamera();

		// Functions that mess with properties of the camera; good old setters
		void setWorldPosition(const glm::vec2& newPos)	{ _worldPosition = newPos; _update = true; }
		void setScale(float newScale)					{ _scale = newScale; _update = true; }
		void setProjMatrix(const glm::mat4& newMat)		{ _projMatrix = newMat; _update = true; }

		// Auxiliary function to cull objects not in view
		bool isSpriteInView(const glm::vec2& pos, const glm::vec2& dim);

		glm::vec2 getWorldPosition()					{ return _worldPosition; }
		float getScale()								{ return _scale; }
		glm::mat4 getProjMatrix()						{ return _projMatrix; }

	private:
		int			_screenWidth, _screenHeight;
		float		_scale;
		bool		_update;
		glm::vec2	_worldPosition;
		glm::mat4	_projMatrix;
		glm::mat4	_orthoMatrix;
	};

}