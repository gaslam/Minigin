#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include "string"
#include <memory>
#include <unordered_map>
#include <typeindex>

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

		template <typename T, typename... Args> T* AddComponent(Args&&... args) {
			if (IsComponentAdded<T>()) {
				return nullptr;
			}

			const std::type_index typeIndex = std::type_index(typeid(T));
			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			auto pointer = component.get();

			m_Components.emplace(typeIndex, std::move(component));

			return pointer;
		};


		//Didn't have time to fix this. I'll wait for feedback to see what can be improved.
		template<typename T>
		T* GetComponent() const
		{
			const std::type_index typeIndex = std::type_index(typeid(T));
			if (!m_Components.at(typeIndex))
			{
				return nullptr;
			}
			auto component = dynamic_cast<T*>(m_Components.at(typeIndex).get());
			return component;
		}
		template<typename T>
		void RemoveComponent()
		{
			const std::type_index typeIndex = std::type_index(typeid(T));
			auto component = m_Components.at(typeIndex);

			if (component)
			{
				m_Components.erase(typeIndex);
			}
		}

		template<typename T>
		bool IsComponentAdded() const
		{
			const std::type_index typeIndex = std::type_index(typeid(T));

			return m_Components.contains(typeIndex);
		}

	private:
		Transform m_transform{};
		std::unordered_map<std::type_index,std::unique_ptr<Component>> m_Components{};
	};
}
