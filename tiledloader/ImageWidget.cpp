#include "ImageWidget.h"

ImageWidget::ImageWidget()
{
	typ = WidgetTyp::Image;
}

ImageWidget::~ImageWidget()
{
}

void ImageWidget::SetImage(LTexture* Image)
{
	image = Image;
}

void ImageWidget::Init(std::string Name, SDL_Rect Rect, LTexture* Image)
{
	name = Name;
	rect = Rect;
	typ = WidgetTyp::Image;
	image = Image;
}

void ImageWidget::Update(float dt)
{
}

void ImageWidget::Render()
{
	image->render(rect.x, rect.y);
}
