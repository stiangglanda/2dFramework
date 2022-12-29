#pragma once
#include "BaseWidget.h"
class ProgressWidget : public BaseWidget
{
public:
	ProgressWidget();
	~ProgressWidget();
	void SetProgress(float Progress);
	float GetProgress();
	void Init(std::string Name, SDL_Rect Rect, SDL_Color Color, float Progress=0);
	void Update(float dt) override;
	void Render() override;
private:
	float progress;
	SDL_Color color;
};

