#pragma once
#include "LTexture.h"
#include <vector>

class animatedSprite
{
public:
	animatedSprite();
	~animatedSprite();
	bool Init(std::string path, float duration, int animationFrames);
	void render(int x, int y);
	void renderSpecificFrame(int x, int y, int frame);
	LTexture* GetTexture();
private:
	LTexture mTexture;
	int mAnimationFrames;
	float mFrame;//float?
	float mDuration;
	std::vector<SDL_Rect> mSpriteClips;
};

