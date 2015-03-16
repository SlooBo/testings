#pragma once

#include <iostream>
#include <vector>

#include "Component.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include <typeinfo>
#include <unordered_map>

class GameObject : public Component
{
public:
	GameObject();
	~GameObject();
	
	//TransformComponent *transformComponent;
	//RenderComponent *renderComponent;
	std::vector<GameObject>gameobjectVector;

	void CreateObject();
	void DeleteObject();
	void AddComponent(GameObject *gameobject, Component component);
	void RemoveComponent(Component component);
	void RemoveAllComponents(GameObject *gameobject);

	Component *GetComponent();

private:

	using ComponentMap = std::unordered_map <const std::type_info*, Component* > ;

	ComponentMap _components;

};

