#include "RenderComponent.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include <string>
#include "Texture2D.h"
#include "GameObject.h"
#include "Transform.h"

dae::RenderComponent::RenderComponent(const std::string& filename)
{
	SetTexture(filename);
}

dae::RenderComponent::RenderComponent(std::shared_ptr<Texture2D> texture)
{
	m_Texture = texture;
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
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
