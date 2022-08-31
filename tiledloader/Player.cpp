#include "Player.h"

void Player::Init()
{
	playerSpriteMoveForward = std::make_unique<animatedSprite>();
	playerSpriteMoveForward->Init("link_sprite_sheetMoveForward.png",15,10);

	playerSpriteMoveRight = std::make_unique<animatedSprite>();
	playerSpriteMoveRight->Init("link_sprite_sheetMoveRight.png", 15, 10);

	playerSpriteMoveBack = std::make_unique<animatedSprite>();
	playerSpriteMoveBack->Init("link_sprite_sheetMoveBack.png", 15, 10);

	playerSpriteMoveLeft = std::make_unique<animatedSprite>();
	playerSpriteMoveLeft->Init("link_sprite_sheetMoveLeft.png", 15, 10);

	playerSpriteIdleForward = std::make_unique<animatedSprite>();
	playerSpriteIdleForward->Init("link_sprite_sheetIdleForward.png", 1, 1);

	playerSpriteIdleRight = std::make_unique<animatedSprite>();
	playerSpriteIdleRight->Init("link_sprite_sheetIdleRight.png", 4, 3);

	playerSpriteIdleBack = std::make_unique<animatedSprite>();
	playerSpriteIdleBack->Init("link_sprite_sheetIdleBack.png", 4, 3);

	playerSpriteIdleLeft = std::make_unique<animatedSprite>();
	playerSpriteIdleLeft->Init("link_sprite_sheetIdleLeft.png", 4, 3);

	mBox.x = 0;
	mBox.y = 0;
	mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;
	mVelX = 0;
	mVelY = 0;
}

void Player::Update()
{
	ActivityInFrame = false;

	if (g_pFramework->KeyDown(SDL_SCANCODE_W))
	{
		y = y- 300.0f * Timer::Get()->GetElapsed();
		mBox.y = y;
		AnimMode = AnimationMode::MoveForward;
		ActivityInFrame = true;
	}
	if (g_pFramework->KeyDown(SDL_SCANCODE_S))
	{
		y = y+300.0f * Timer::Get()->GetElapsed();
		mBox.y = y;
		AnimMode = AnimationMode::MoveBack;
		ActivityInFrame = true;
	}
	if (g_pFramework->KeyDown(SDL_SCANCODE_A))
	{
		x = x - 300.0f * Timer::Get()->GetElapsed();
		mBox.x = x;
		AnimMode = AnimationMode::MoveLeft;
		ActivityInFrame = true;
	}
	if (g_pFramework->KeyDown(SDL_SCANCODE_D))
	{
		x = x+ 300.0f * Timer::Get()->GetElapsed();
		mBox.x = x;
		AnimMode = AnimationMode::MoveRight;
		ActivityInFrame = true;
	}

}

void Player::Draw(SDL_Rect& camera)
{
	/*playerSprite.get()->xcount =- camera.x;
	playerSprite.get()->ycount =- camera.y;*/

	if (!ActivityInFrame)
	{
		switch (AnimMode)
		{
		case AnimationMode::MoveForward:
			AnimMode = AnimationMode::IdleForward;
			break;
		case AnimationMode::MoveRight:
			AnimMode = AnimationMode::IdleRight;
			break;
		case AnimationMode::MoveBack:
			AnimMode = AnimationMode::IdleBack;
			break;
		case AnimationMode::MoveLeft:
			AnimMode = AnimationMode::IdleLeft;
			break;
		default:
			break;
		}
	}

	switch (AnimMode)
	{
	case AnimationMode::MoveForward:
		playerSpriteMoveForward.get()->render(mBox.x - camera.x, mBox.y - camera.y);
		break;
	case AnimationMode::MoveRight:
		playerSpriteMoveRight.get()->render(mBox.x - camera.x, mBox.y - camera.y);
		break;
	case AnimationMode::MoveBack:
		playerSpriteMoveBack.get()->render(mBox.x - camera.x, mBox.y - camera.y);
		break;
	case AnimationMode::MoveLeft:
		playerSpriteMoveLeft.get()->render(mBox.x - camera.x, mBox.y - camera.y);
		break;
	case AnimationMode::IdleForward:
		playerSpriteIdleForward.get()->render(mBox.x - camera.x, mBox.y - camera.y);
		break;
	case AnimationMode::IdleRight:
		playerSpriteIdleRight.get()->render(mBox.x - camera.x, mBox.y - camera.y);
		break;
	case AnimationMode::IdleBack:
		playerSpriteIdleBack.get()->render(mBox.x - camera.x, mBox.y - camera.y);
		break;
	case AnimationMode::IdleLeft:
		playerSpriteIdleLeft.get()->render(mBox.x - camera.x, mBox.y - camera.y);
		break;
	default:
		break;
	}
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