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

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("background.tga");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	auto renderComp2 = go->AddComponent<dae::RenderComponent>("logo.tga");
	renderComp2->SetPosition(216, 180);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go->AddComponent<dae::RenderComponent>();
	auto text = go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	text->Initialise();
	text->SetPosition(80, 20);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	auto fps = go->AddComponent<dae::FPSComponent>();
	go->AddComponent<dae::RenderComponent>();
	text = go->AddComponent<dae::TextComponent>("0", font);

	fps->Initialise();
	text->Initialise();
	
	scene.Add(go);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}