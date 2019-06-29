#pragma once
#include "Errors.h"
namespace Denjine{

	class IOManager
	{
	public:
		static bool readFileIntoBuffer(std::string filePath, std::vector<unsigned char> &buf);

		static GLuint loadTextureFromFile(std::string filePath);
	};

}