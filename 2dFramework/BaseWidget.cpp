#include "BaseWidget.h"

BaseWidget::BaseWidget()
{
	typ = WidgetTyp::Base;
}

BaseWidget::~BaseWidget()
{
}

void BaseWidget::Init(std::string Name, SDL_Rect Rect)
{
	name = Name;
	rect = Rect;
	typ = WidgetTyp::Base;
}

void BaseWidget::Update(float dt)
{
}

void BaseWidget::handleEvent(SDL_Event* e)
{
}

void BaseWidget::Render()
{
}

WidgetTyp BaseWidget::GetTyp()
{
	return typ;
}
