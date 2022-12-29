#pragma once
#include <vector>
#include <memory>
#include "BaseWidget.h"
#include "LabelWidget.h"
#include "animatedSprite.h"
#include <functional>

class GUISystem
{
public:
	GUISystem();
	~GUISystem();
	void Init();
	void Update(float dt);
	void handleEvent(SDL_Event* e);
	void Render();
	void AddWidget(BaseWidget* Widget);
	void CreateLabel(std::string Name, SDL_Rect Rect, std::string Label, SDL_Color Color);
	void CreateButton(std::string Name, SDL_Rect Rect, std::string Label, SDL_Color Color, std::function<void()> OnClick = nullptr);
	void CreateProgress(std::string Name, SDL_Rect Rect, SDL_Color Color, float Progress=0);
	void CreateImage(std::string Name, SDL_Rect Rect, LTexture* Image);
private:
	std::vector<BaseWidget*> widgets;
};

