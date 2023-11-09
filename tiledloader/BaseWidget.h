#pragma once
#include "LTexture.h"

enum class WidgetTyp
{
	Base,
	Label,
	Button,
	Image,
	Progress
};

class BaseWidget
{
public:
	BaseWidget();
	~BaseWidget();
	virtual void Init(std::string Name, SDL_Rect Rect);
	virtual void Update(float dt);
	virtual void handleEvent(SDL_Event* e);
	virtual void Render();
	WidgetTyp GetTyp();

protected:
	std::string name;
	SDL_Rect rect;
	WidgetTyp typ;
	bool render;
};
