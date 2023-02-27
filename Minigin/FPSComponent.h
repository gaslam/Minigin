#pragma once
#include "Component.h"
#include "Transform.h"
#include <string>
#include <memory>
namespace dae {
    class Font;
    class RenderComponent;
    class Texture2D;
    class Font;

    class FPSComponent :
        public Component
    {
    public:
        explicit FPSComponent(std::shared_ptr<Font> font);
        void Render() const override;
        void Update(dae::GameObject& object, float deltaTime) override;
    private:
        bool m_NeedsUpdate;
        int m_CountFPS{};
        float m_AccumulatedTime{};
        float sec{ 1.0f };
        std::string m_FPSValue{ "0" };
        std::shared_ptr<Font> m_Font;
        std::shared_ptr<RenderComponent> m_TextRenderComp;
        Transform m_Transform;
    };
}

