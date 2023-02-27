#pragma once
#include <glm/glm.hpp>
#include "GameObject.h"
class Component {
public:
	Component() = default;
	virtual ~Component() = default;

	Component(const Component& other) = delete;
	Component(Component&& other) = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) = delete;
	virtual void Render() const = 0;
	virtual void Update(dae::GameObject& object, float deltaTime) = 0;
};