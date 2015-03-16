#pragma once

#include "GameObject.h"
#include "lodepng.h"
#include "glew.h"

class RenderComponent :
	public GameObject
{
public:
	RenderComponent();
	~RenderComponent();

	void InitTexture2d(GLint texName, GLint texWidth, GLint texHeight, GLubyte *texPtr);
	void SetData(const char* path);
	void Draw();
	unsigned GetData();

private:
	unsigned TextureData, width, height;
	GLuint texture;
};

