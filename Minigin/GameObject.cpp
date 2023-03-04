#include <string>
#include "GameObject.h"
#include "Renderer.h"
#include "RenderComponent.h"
#include "Texture2D.h"
#include <algorithm>
#include "Component.h"


dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime)
{
	for (auto& component : m_Components)
	{
		component.second->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{

	for (auto& component : m_Components)
	{
		component.second->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
