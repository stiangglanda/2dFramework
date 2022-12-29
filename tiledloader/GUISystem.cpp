#include "GUISystem.h"
#include "ButtonWidget.h"
#include "ProgressWidget.h"
#include "ImageWidget.h"

GUISystem::GUISystem()
{
}

GUISystem::~GUISystem()
{
	for(auto i : widgets)
	{
		i->~BaseWidget();
	}
}

void GUISystem::Init()
{
}

void GUISystem::Update(float dt)
{
	for(auto i: widgets)
	{
		switch (i->GetTyp())
		{
		case WidgetTyp::Base:
		{
			i->Update(dt);
			break;
		}
		case WidgetTyp::Label:
		{
			//static_cast<LabelWidget*>(i.get())->Update(dt);
			static_cast<LabelWidget*>(i)->Update(dt);
			break;
		}
		default:
			break;
		}
	}
}

void GUISystem::handleEvent(SDL_Event* e)
{
	for (auto i : widgets)
	{
		switch (i->GetTyp())
		{
		case WidgetTyp::Button:
		{
			static_cast<ButtonWidget*>(i)->handleEvent(e);
			break;
		}
		default:
			break;
		}
	}
}

void GUISystem::Render()
{
	for (auto i : widgets)
	{
		switch (i->GetTyp())
		{
		case WidgetTyp::Base:
		{
			i->Render();
			break;
		}
		case WidgetTyp::Label:
		{
			//static_cast<LabelWidget*>(i.get())->Render();
			//std::static_pointer_cast<LabelWidget>(i)->Render();
			static_cast<LabelWidget*>(i)->Render();
			break;
		}
		case WidgetTyp::Button:
		{
			//static_cast<LabelWidget*>(i.get())->Render();
			//std::static_pointer_cast<LabelWidget>(i)->Render();
			static_cast<ButtonWidget*>(i)->Render();
			break;
		}
		case WidgetTyp::Progress:
		{
			//static_cast<LabelWidget*>(i.get())->Render();
			//std::static_pointer_cast<LabelWidget>(i)->Render();
			static_cast<ProgressWidget*>(i)->Render();
			break;
		}
		case WidgetTyp::Image:
		{
			//static_cast<LabelWidget*>(i.get())->Render();
			//std::static_pointer_cast<LabelWidget>(i)->Render();
			static_cast<ImageWidget*>(i)->Render();
			break;
		}
		default:
			break;
		}
	}
}

void GUISystem::AddWidget(BaseWidget* Widget)
{
	widgets.push_back(Widget);
}

void GUISystem::CreateLabel(std::string Name, SDL_Rect Rect, std::string Label, SDL_Color Color)
{
	LabelWidget* wid = new LabelWidget();
	wid->Init(Name, Rect, Label, Color);
	AddWidget(wid);
}

void GUISystem::CreateButton(std::string Name, SDL_Rect Rect, std::string Label, SDL_Color Color, std::function<void()> OnClick)
{
	ButtonWidget* wid= new ButtonWidget();
	wid->Init(Name, Rect, Label, Color, OnClick);
	AddWidget(wid);
}

void GUISystem::CreateProgress(std::string Name, SDL_Rect Rect, SDL_Color Color, float Progress)
{
	ProgressWidget* wid = new ProgressWidget();
	wid->Init(Name, Rect, Color, Progress);
	AddWidget(wid);
}

void GUISystem::CreateImage(std::string Name, SDL_Rect Rect, LTexture* Image)
{
	ImageWidget* wid = new ImageWidget();
	wid->Init(Name, Rect, Image);
	AddWidget(wid);
}
