#include "LabelWidget.h"

LabelWidget::LabelWidget()
{
	typ = WidgetTyp::Label;
}

LabelWidget::~LabelWidget()
{
	texture.free();
}

void LabelWidget::SetLabel(std::string Label)
{
	label = Label;

	if (!texture.loadFromRenderedText(label, color))
	{
		printf("Failed to render text texture!\n");
	}
}

void LabelWidget::Init(std::string Name, SDL_Rect Rect, std::string Label, SDL_Color Color)
{
	name = Name;
	rect = Rect;
	typ = WidgetTyp::Label;
	label = Label;
	color = Color;

	if (!texture.loadFromRenderedText(label, color))
	{
		printf("Failed to render text texture!\n");
	}
}

void LabelWidget::Update(float dt)
{
}

void LabelWidget::Render()
{
	texture.render(rect.x, rect.y);
}
