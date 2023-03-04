#include "RenderComponent.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include <string>
#include "Texture2D.h"
#include "GameObject.h"

dae::RenderComponent::RenderComponent(GameObject* owner,const std::string& filename) : Component(owner), m_Transform{}
{
	SetTexture(filename);
}

dae::RenderComponent::RenderComponent(GameObject* owner, std::shared_ptr<Texture2D> texture) : Component{ owner }, m_Transform{}
{
	m_Texture = texture;
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetTexture(const std::shared_ptr<Texture2D> texture)
{
	m_Texture = texture;
}

void dae::RenderComponent::Render() const
{
	const auto pos{ m_Transform.GetPosition() };
	dae::Renderer::GetInstance().RenderTexture(*m_Texture,pos.x,pos.y);
}

void dae::RenderComponent::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.f);
}
