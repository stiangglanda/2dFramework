#include "Player.h"

void Player::Init()
{
	playerSprite = std::make_unique<animatedSprite>();
	playerSprite->Init("foo.png",15,4);

	mBox.x = 0;
	mBox.y = 0;
	mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;
	mVelX = 0;
	mVelY = 0;
}

void Player::Update()
{
	if (g_pFramework->KeyDown(SDL_SCANCODE_W))
	{
		y = y- 300.0f * Timer::Get()->GetElapsed();
		mBox.y = y;
	}
	if (g_pFramework->KeyDown(SDL_SCANCODE_A))
	{
		x = x- 300.0f * Timer::Get()->GetElapsed();
		mBox.x = x;
	}
	if (g_pFramework->KeyDown(SDL_SCANCODE_S))
	{
		y = y+300.0f * Timer::Get()->GetElapsed();
		mBox.y = y;
	}
	if (g_pFramework->KeyDown(SDL_SCANCODE_D))
	{
		x = x+ 300.0f * Timer::Get()->GetElapsed();
		mBox.x = x;
	}

}

void Player::Draw(SDL_Rect& camera)
{
	/*playerSprite.get()->xcount =- camera.x;
	playerSprite.get()->ycount =- camera.y;*/
	playerSprite.get()->render(mBox.x - camera.x, mBox.y - camera.y);
}

void Player::Quit()
{

}

void Player::setCamera(SDL_Rect& camera, float levelWidth,float levelHight)
{
	//Center the camera over the dot
	camera.x = (mBox.x + DOT_WIDTH / 2) - g_pFramework->GetScreenWidth() / 2;
	camera.y = (mBox.y + DOT_HEIGHT / 2) - g_pFramework->GetScreenHeight() / 2;

	//Keep the camera in bounds
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > levelWidth - camera.w)//TODO 900 and 700 is level size
	{
		camera.x = levelWidth - camera.w;
	}
	if (camera.y > levelHight - camera.h)
	{
		camera.y = levelHight - camera.h;
	}
}