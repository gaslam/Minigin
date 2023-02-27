#pragma once
#include "Component.h"
#include <memory>
#include "string"
namespace dae {
	class Texture2D;
	class Transform;
	class RenderComponent : public Component
	{
	public:
		explicit RenderComponent(std::shared_ptr<Texture2D>texture);
		explicit RenderComponent(const std::string& filename);
		virtual void Render() const override;
		virtual void Update(dae::GameObject&,float) override {};
		void SetTexture(const std::string& filename);
		std::shared_ptr<Texture2D> GetTexture() const { return m_Texture; }
		void SetPosition(float x, float y);
	private:
		std::shared_ptr<Texture2D> m_Texture{};
		Transform m_Transform{};
	};
}

