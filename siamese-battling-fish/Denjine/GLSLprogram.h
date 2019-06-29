#pragma once

#include "Headerz.h"
#include "Errors.h"


//This class handles the compilation, linking, and usage of a GLSL shader program.
namespace Denjine{

	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilepath);

		void linkShaders();


		GLint	getUniformLocation(const std::string& uniformName);	// To get a handle on the uniform
		void	addAttribute(const std::string& attributeName);		// to add another attribute to our shader program

		void use();
		void unuse();
	private:

		int _numAttributes;

		void compileShader(const std::string& filePath, GLuint id);

		GLuint _programID;

		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
	};
}
