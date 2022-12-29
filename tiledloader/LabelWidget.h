#pragma once
#include "BaseWidget.h"
class LabelWidget : public BaseWidget
{
public:
	LabelWidget();
	~LabelWidget();
	void SetLabel(std::string Label);
	void Init(std::string Name, SDL_Rect Rect, std::string Label,SDL_Color Color);
	void Update(float dt) override;
	void Render() override;
private:
	std::string label;
	SDL_Color color;
	LTexture texture;
};

