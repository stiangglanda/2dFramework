#pragma once
#include "BaseWidget.h"
class ImageWidget : public BaseWidget
{
public:
	ImageWidget();
	~ImageWidget();
	void SetImage(LTexture* Image);
	void Init(std::string Name, SDL_Rect Rect, LTexture* Image);
	void Update(float dt) override;
	void Render() override;
private:
	LTexture* image;
};

