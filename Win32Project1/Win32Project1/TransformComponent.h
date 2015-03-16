#pragma once
#include "GameObject.h"

#include <vector>
#include "glm\vec2.hpp"

GLfloat vertexData[]
{
	0.0, 0.0,
	0.0, 1.0,
	1.0, 0.0,
	
	0.0, 1.0,
	1.0, 0.0,
	1.0, 1.0
};

class TransformComponent :
	public GameObject
{
public:
	TransformComponent();
	~TransformComponent();

	void SetPosition(glm::vec2 position){ _position = position; }
	glm::vec2 GetPosition() { return _position; }
	void SetSize(glm::vec2 size);
	glm::vec2 GetSize(){ return _size; };

private:
	glm::vec2 _position, _size;
};

