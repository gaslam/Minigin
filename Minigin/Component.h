#pragma once
#include <glm/glm.hpp>
#include "GameObject.h"
class Component {
	dae::GameObject* m_pOwner{ nullptr };
public:
	virtual ~Component() = default;

	Component(const Component& other) = delete;
	Component(Component&& other) = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) = delete;
	virtual void Render() const = 0;
	virtual void Update(float deltaTime) = 0;
protected:
	dae::GameObject* GetOwner() const { return m_pOwner; }
	explicit Component(dae::GameObject* owner) : m_pOwner{ owner } {};
};