#pragma once

#include <vector>

class Component
{
public:
	Component();
	~Component();
protected:
	std::vector<Component>ComponentVector;

	void NewComponent(std::string name) {}
	void DeleteComponent(Component component){};

private:
	std::string name;
};