#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
//#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include <memory>
#include "RenderComponent.h"
#include "FPSComponent.h"
#include "RotationComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("background.tga");
	auto transform = go->AddComponent<dae::Transform>();
	scene.Add(go);

		go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("logo.tga");
	transform = go->AddComponent<dae::Transform>();
	transform->SetLocalPosition({ 216, 180,0 });
	scene.Add(go);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}