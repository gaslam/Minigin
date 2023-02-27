#include "FPSComponent.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include <glm/glm.hpp>
#include "Font.h"
#include "Renderer.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "Texture2D.h"

void dae::FPSComponent::Update(GameObject& object, float deltaTime)
{
	m_AccumulatedTime += deltaTime;
	++m_CountFPS;
	if (m_AccumulatedTime > sec)
	{
		m_AccumulatedTime -= sec;
		m_FPSValue = std::to_string(m_CountFPS);
		m_CountFPS = 0;
		m_NeedsUpdate = true;
	}

	if (m_NeedsUpdate)
	{
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_FPSValue.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		auto renderComponent = std::make_shared<RenderComponent>(std::make_shared<Texture2D>(texture));
		if (object.IsComponentAdded<RenderComponent>())
		{
			object.RemoveComponent<RenderComponent>();
		}
		object.AddComponent<RenderComponent>(renderComponent);
		m_TextRenderComp = renderComponent;

		m_NeedsUpdate = false;
	}
}

dae::FPSComponent::FPSComponent(std::shared_ptr<Font> font) :
	m_Font{ font }, m_Transform{}, m_NeedsUpdate{ true }
{
}

void dae::FPSComponent::Render() const
{
	if (m_TextRenderComp == nullptr)
	{
		return;
	}

	auto texture = m_TextRenderComp->GetTexture();
	if (texture != nullptr)
	{
		const glm::vec3 pos{ m_Transform.GetPosition() };
		Renderer::GetInstance().RenderTexture(*texture, pos.x, pos.y);
	}
}
