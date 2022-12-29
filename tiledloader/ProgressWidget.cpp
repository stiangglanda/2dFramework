#include "ProgressWidget.h"
#include "Framework.h"

ProgressWidget::ProgressWidget()
{
	progress = 0;
	typ = WidgetTyp::Progress;
}

ProgressWidget::~ProgressWidget()
{
}

void ProgressWidget::SetProgress(float Progress)
{
	progress = Progress;
}

float ProgressWidget::GetProgress()
{
	return progress;
}

void ProgressWidget::Init(std::string Name, SDL_Rect Rect, SDL_Color Color, float Progress)
{
	name = Name;
	rect = Rect;
	typ = WidgetTyp::Progress;
	color = Color;
	progress = Progress;
}

void ProgressWidget::Update(float dt)
{
}

void ProgressWidget::Render()
{

	SDL_SetRenderDrawColor(g_pFramework->GetRenderer(), color.r, color.g, color.b, 0xFF);
	SDL_Rect ProgressRect=rect;
	ProgressRect.w=((float)rect.w / 100) * (progress*100);
	SDL_RenderFillRect(g_pFramework->GetRenderer(), &ProgressRect);



	SDL_SetRenderDrawColor(g_pFramework->GetRenderer(), 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(g_pFramework->GetRenderer(), &rect);
}
