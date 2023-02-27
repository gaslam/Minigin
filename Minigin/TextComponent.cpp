#include <stdexcept>
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "RenderComponent.h"

	dae::TextComponent::TextComponent(const std::string& text, std::shared_ptr<Font> font)
		: m_NeedsUpdate(true), m_text(text), m_font(std::move(font)), m_RenderComponent(nullptr)
	{ }

	void dae::TextComponent::Update(dae::GameObject& object,float)
	{
		if (m_NeedsUpdate)
		{
			const SDL_Color color = { 255,255,255 }; // only white text is supported now
			const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
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
			auto pos = m_transform.GetPosition();
			renderComponent->SetPosition(pos.x, pos.y);
			if (object.IsComponentAdded<RenderComponent>())
			{
				object.RemoveComponent<RenderComponent>();
			}
			object.AddComponent<RenderComponent>(renderComponent);
			m_RenderComponent = renderComponent;

			m_NeedsUpdate = false;
		}
	}

	// This implementation uses the "dirty flag" pattern
	void dae::TextComponent::SetText(const std::string& text)
	{
		m_text = text;
		m_NeedsUpdate = true;
	}

	void dae::TextComponent::SetPosition(const float x, const float y)
	{
		m_transform.SetPosition(x, y, 0.0f);
	}


