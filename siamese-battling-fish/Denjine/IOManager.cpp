#include "IOManager.h"
#include "Errors.h"
namespace Denjine{

	bool IOManager::readFileIntoBuffer(std::string filePath, std::vector<unsigned char>& buffer) {
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}

		//seek to the end
		file.seekg(0, std::ios::end);

		//Get the file size
		int fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		//Reduce the file size by any header bytes that might be present
		fileSize -= file.tellg();

		buffer.resize(fileSize);
		file.read((char *)&(buffer[0]), fileSize);
		file.close();

		return true;
	}

	GLuint IOManager::loadTextureFromFile(std::string filePath)
	{
		// SOIL version
			GLuint tex = 0;
			tex = SOIL_load_OGL_texture(
			filePath.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y);

			//Bind the texture object
			glBindTexture(GL_TEXTURE_2D, tex);
			//Upload the pixels to the texture

			//Set some texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			//Generate the mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);

			//Unbind the texture
			glBindTexture(GL_TEXTURE_2D, 0);
			//	glGenerateMipmap(GL_TEXTURE_2D);
			if (!tex)
			{
			fatalError(("SOIL could not load texture; error '%s'\n", SOIL_last_result()));
			}


			return tex; 

		// picoPNG version

		// Create a GLTexture and initialize all its fields to 0
	/*	GLuint texture = 0;

		//This is the input data to decodePNG, which we load from a file
		std::vector<unsigned char> in;
		//This is the output data from decodePNG, which is the pixel data for our texture
		std::vector<unsigned char> out;

		unsigned long width, height;

		//Read in the image file contents into a buffer
		if (readFileIntoBuffer(filePath, in) == false) {
			fatalError("Failed to load PNG file to buffer!");
		}

		//Decode the .png format into an array of pixels
		int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
		if (errorCode != 0) {
			fatalError("decodePNG failed with error: " + std::to_string(errorCode));
		}

		//Generate the openGL texture object
		glGenTextures(1, &texture);

		//Bind the texture object
		glBindTexture(GL_TEXTURE_2D, texture);
		//Upload the pixels to the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		//Set some texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//Generate the mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);

		//Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		//Return a copy of the texture data
		return texture; */

	}

}