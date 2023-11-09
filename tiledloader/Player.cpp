#include "Player.h"

void Player::Init()
{
	playerSpriteMoveForward = std::make_unique<animatedSprite>();
	playerSpriteMoveForward->Init("harrypotter/link_sprite_sheetMoveForward.png", 15, 9);

	playerSpriteMoveRight = std::make_unique<animatedSprite>();
	playerSpriteMoveRight->Init("harrypotter/link_sprite_sheetMoveRight.png", 15, 9);

	playerSpriteMoveBack = std::make_unique<animatedSprite>();
	playerSpriteMoveBack->Init("harrypotter/link_sprite_sheetMoveBack.png", 15, 9);

	playerSpriteMoveLeft = std::make_unique<animatedSprite>();
	playerSpriteMoveLeft->Init("harrypotter/link_sprite_sheetMoveLeft.png", 15, 9);

	playerSpriteIdleForward = std::make_unique<animatedSprite>();
	playerSpriteIdleForward->Init("harrypotter/link_sprite_sheetIdleForward.png", 1, 1);

	playerSpriteIdleRight = std::make_unique<animatedSprite>();
	playerSpriteIdleRight->Init("harrypotter/link_sprite_sheetIdleRight.png", 1, 1);

	playerSpriteIdleBack = std::make_unique<animatedSprite>();
	playerSpriteIdleBack->Init("harrypotter/link_sprite_sheetIdleBack.png", 1, 1);

	playerSpriteIdleLeft = std::make_unique<animatedSprite>();
	playerSpriteIdleLeft->Init("harrypotter/link_sprite_sheetIdleLeft.png", 1, 1);

	mBox.x = 0;
	mBox.y = 0;
	mBox.w = 23;
	mBox.h = 44;
	mVelX = 0;
	mVelY = 0;
}

void Player::Update(const std::vector<SDL_Rect>* collisionLayer)
{
	ActivityInFrame = false;
	SDL_Rect oldCoord = mBox;

	if (g_pFramework->KeyDown(SDL_SCANCODE_W))
	{
		y = y - 300.0f * Timer::Get()->GetElapsed();
		mBox.y = y;
		AnimMode = AnimationMode::MoveForward;
		ActivityInFrame = true;
	}
	if (g_pFramework->KeyDown(SDL_SCANCODE_S))
	{
		y = y + 300.0f * Timer::Get()->GetElapsed();
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
		x = x + 300.0f * Timer::Get()->GetElapsed();
		mBox.x = x;
		AnimMode = AnimationMode::MoveRight;
		ActivityInFrame = true;
	}

	if (g_pFramework->touchesWall(mBox, collisionLayer))
	{
		mBox = oldCoord;
		x = mBox.x;
		y = mBox.y;
	}
}

int Player::GetX()
{
	return mBox.x;
}

int Player::GetY()
{
	return mBox.y;
}

SDL_Rect Player::GetAABB()
{
	return mBox;
}

void Player::Draw(SDL_Rect& camera)
{
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

	g_pFramework->RenderRect(mBox, camera);
}

void Player::Quit()
{
}

void Player::setCamera(SDL_Rect& camera, float levelWidth, float levelHight)
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
	if (camera.x > levelWidth - camera.w) //TODO 900 and 700 is level size
	{
		camera.x = levelWidth - camera.w;
	}
	if (camera.y > levelHight - camera.h)
	{
		camera.y = levelHight - camera.h;
	}
}
