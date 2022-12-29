#pragma once
#include "BaseWidget.h"
#include "Framework.h"
#include <functional>
#include <memory>
enum class ButtonStatus
{
	MOUSE_OUT = 0,
	MOUSE_OVER = 1,
	MOUSE_DOWN = 2,
	MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};


class ButtonWidget : public BaseWidget
{
public:
		ButtonWidget();
		~ButtonWidget();
		void SetLabel(std::string Label);
		void Init(std::string Name, SDL_Rect Rect, std::string Label, SDL_Color Color, std::function<void()> OnClick=nullptr);
		void Update(float dt) override;
		void Render() override;
		void handleEvent(SDL_Event* e) override;
		void SetOnClick(std::function<void()> OnClick);
private:
	std::string label;
	SDL_Color color;
	//animatedSprite* texture;
	//LTexture textureNormal;
	//LTexture textureHover;
	//LTexture textureSelected;
	LTexture textTexture;
	ButtonStatus status;
	std::function<void()> onClick;
};

