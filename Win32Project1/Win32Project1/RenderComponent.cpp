#include "RenderComponent.h"


RenderComponent::RenderComponent()
{
}


RenderComponent::~RenderComponent()
{
}

void RenderComponent::InitTexture2d(GLint texName, GLint texWidth, GLint texHeight, GLubyte *texPtr)
{
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texPtr);
	glBindTexture(GL_TEXTURE_2D, 0u);
}

void RenderComponent::SetData(const char* path)
{
	glGenTextures(1, &texture);

	std::vector<unsigned char>data;
	TextureData = lodepng::decode(data, width, height, path);
	InitTexture2d(texture, width, height, data.data);
}

unsigned RenderComponent::GetData()
{
	return TextureData;
}

void RenderComponent::Draw()
{
	glActiveTexture(GL_TEXTURE0 + texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
}

