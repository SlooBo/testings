#include "GameObject.h"

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::CreateObject()
{
	GameObject *gameobject = new GameObject;
	gameobjectVector.push_back(*gameobject);
}

void GameObject::AddComponent(GameObject *gameobject, Component component)
{

}

void GameObject::RemoveComponent(Component component)
{

}

void GameObject::RemoveAllComponents(GameObject *gameobject)
{

}