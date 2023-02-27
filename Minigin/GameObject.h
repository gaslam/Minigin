#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include "string"
#include <memory>

class Component;
namespace dae
{

	// todo: this should become final.
	class GameObject final
	{
	public:
		virtual void Update(float deltaTime);
		virtual void Render() const;

		void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template<typename T>
		void AddComponent(std::shared_ptr<T> component)
		{
			if (!IsComponentAdded<T>())
			{
				m_Components.emplace_back(component);
			}
		}


		//Didn't have time to fix this. I'll wait for feedback to see what can be improved.
		template<typename T>
		std::shared_ptr<Component> GetComponent() const
		{
			for (auto& component : m_Components)
			{
				if (typeid(T) == typeid(*component.get()))
				{
					return component;
				}
			}
			return nullptr;
		}
		template<typename T>
		void RemoveComponent()
		{
			auto it = std::remove_if(m_Components.begin(), m_Components.end(), [&](std::shared_ptr<Component> comp1) {
				if (typeid(T) == typeid(*comp1.get()))
				{
					return true;
				}
			return false;
				});

			if (it != m_Components.end())
			{
				m_Components.erase(it);
			}
		}

		template<typename T>
		bool IsComponentAdded() const
		{
			auto it = std::find_if(m_Components.begin(), m_Components.end(), [&](std::shared_ptr<Component> comp1) {
				if (typeid(T) == typeid(*comp1.get()))
				{
					return true;
				}
			return false;
				});

			return it != m_Components.end();
		}

	private:
		Transform m_transform{};
		// todo: mmm, every gameobject has a texture? Is that correct?
		std::vector<std::shared_ptr<Component>> m_Components{};
	};
}
