#include "animatedSprite.h"
#include "Timer.h"

animatedSprite::animatedSprite()
{
}

animatedSprite::~animatedSprite()
{
	mTexture.free();
}

bool animatedSprite::Init(std::string path, float duration, int animationFrames)
{
	bool status = mTexture.loadFromFile(path);
	mAnimationFrames = animationFrames;
	mDuration = duration;
	mFrame = 0;

	int TextureHight = mTexture.getHeight();
	int TextureWidth = mTexture.getWidth();
	int TexturWidthPerFrame = TextureWidth / animationFrames;

	for (size_t i = 0; i < animationFrames; i++)
	{
		SDL_Rect rect;
		rect.x = i * TexturWidthPerFrame;
		rect.y = 0;
		rect.w = TexturWidthPerFrame;
		rect.h = TextureHight;
		mSpriteClips.push_back(rect);
	}


	return status;
}

void animatedSprite::render(int x, int y)
{
	if (mFrame > mAnimationFrames)
	{
		mFrame = 0;
	}
	mTexture.render(x, y, &mSpriteClips[mFrame]);

	mFrame += mDuration * Timer::Get()->GetElapsed();
}

void animatedSprite::renderSpecificFrame(int x, int y, int frame)
{
	mTexture.render(x, y, &mSpriteClips[frame]);
}

LTexture* animatedSprite::GetTexture()
{
	return &mTexture;
}
