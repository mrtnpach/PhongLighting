#include "STBLoading.h"
#include <glad/glad.h>
#include "stb_image.h"
#include "LabDebug.h"

void load2DTexture(unsigned int& texture, const char* path, bool isTransparent, bool& loadingSuccessful)
{
	// Generate and bind texture object
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Use mipmaps for filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Using mipmap for magnification is useless

	int textureWidth;
	int textureHeight;
	int numberOfChannels;

	stbi_set_flip_vertically_on_load(true);
	// Load image as "raw" data
	unsigned char* data = stbi_load(path, &textureWidth, &textureHeight, &numberOfChannels, 0);
	if (data)
	{
		unsigned int imageChannels = isTransparent ? GL_RGBA : GL_RGB;
		// Generate texture image using bound texture object
		glTexImage2D(
			GL_TEXTURE_2D, // Generate a 2D texture
			0, // Mipmap level
			GL_RGB, textureWidth, textureHeight, // Format and dimensions
			0, // Legacy stuff
			imageChannels, GL_UNSIGNED_BYTE, // The image's format and data type (char = byte)
			data); // Image data loaded using stb
		glGenerateMipmap(texture);
		loadingSuccessful = true;
	}
	else
	{
		ShowLabMessage("STBImage:ERROR -> Failed to load texture");
		loadingSuccessful = false;
	}
	stbi_image_free(data);
}