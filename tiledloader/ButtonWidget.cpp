#include "ButtonWidget.h"

ButtonWidget::ButtonWidget()
{
	typ = WidgetTyp::Button;
	status = ButtonStatus::MOUSE_OUT;
	onClick = nullptr;
}

ButtonWidget::~ButtonWidget()
{
	textTexture.free();
}

void ButtonWidget::SetLabel(std::string Label)
{
	label = Label;

	if (!textTexture.loadFromRenderedText(label, color))
	{
		printf("Failed to render text texture!\n");
	}
}

void ButtonWidget::Init(std::string Name, SDL_Rect Rect, std::string Label, SDL_Color Color, std::function<void()> OnClick)
{
	name = Name;
	rect = Rect;
	typ = WidgetTyp::Button;
	label = Label;
	color = Color;
	status = ButtonStatus::MOUSE_OUT;
	onClick = OnClick;

	if (!textTexture.loadFromRenderedText(label, color))
	{
		printf("Failed to render text texture!\n");
	}
}

void ButtonWidget::Update(float dt)
{
}

void ButtonWidget::Render()
{
	SDL_SetRenderDrawColor(g_pFramework->GetRenderer(), 0xFF, 0x00, 0x00, 0xFF);
	switch (status)
	{
	case ButtonStatus::MOUSE_OUT:
		SDL_SetRenderDrawColor(g_pFramework->GetRenderer(), 0xFF, 0x00, 0x00, 0xFF);
		break;
	case ButtonStatus::MOUSE_OVER:
		SDL_SetRenderDrawColor(g_pFramework->GetRenderer(), 0x00, 0xFF, 0x00, 0xFF);
		break;
	case ButtonStatus::MOUSE_DOWN:
		SDL_SetRenderDrawColor(g_pFramework->GetRenderer(), 0x00, 0x00, 0xFF, 0xFF);
		break;
	case ButtonStatus::MOUSE_UP:
		SDL_SetRenderDrawColor(g_pFramework->GetRenderer(), 0x00, 0x00, 0x00, 0xFF);
		break;
	case ButtonStatus::BUTTON_SPRITE_TOTAL:
		break;
	default:
		break;
	}
	
	SDL_RenderDrawRect(g_pFramework->GetRenderer(), &rect);

	int TextXCord, TextYCord=0;

	TextXCord = rect.x + rect.w / 2 - textTexture.getWidth() / 2;
	TextYCord = rect.y + rect.h / 2 - textTexture.getHeight() / 2;

	textTexture.render(TextXCord, TextYCord);
	//textTexture.render(rect.x, rect.y);
}

void ButtonWidget::handleEvent(SDL_Event* e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);
		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < rect.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > rect.x + rect.w)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < rect.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > rect.y + rect.h)
		{
			inside = false;
		}
		//Mouse is outside button
		if (!inside)
		{
			status = ButtonStatus::MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				status = ButtonStatus::MOUSE_OVER;
				break;

			case SDL_MOUSEBUTTONDOWN:
				status = ButtonStatus::MOUSE_DOWN;
				if (onClick != nullptr)
				{
					onClick();
				}
				break;

			case SDL_MOUSEBUTTONUP:
				status = ButtonStatus::MOUSE_UP;
				break;
			}
		}
	}
}

void ButtonWidget::SetOnClick(std::function<void()> OnClick)
{
	onClick = OnClick;
}
