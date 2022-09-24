#pragma once
#include "Sprite.h"
#include "LTexture.h"
#include "animatedSprite.h"

enum class AnimationMode{
	MoveForward,
	MoveRight,
	MoveBack,
	MoveLeft,
	IdleForward,
	IdleRight,
	IdleBack,
	IdleLeft
};

class Player
{
public:
	void Init();
	void Draw(SDL_Rect& camera);
	void Update(const std::vector<SDL_Rect>* collisionLayer);
	int GetX();
	int GetY();
	SDL_Rect GetAABB();
	void Quit();
	void setCamera(SDL_Rect& camera, float levelWidth, float levelHight);
private:
	std::unique_ptr<animatedSprite> playerSpriteMoveForward;
	std::unique_ptr<animatedSprite> playerSpriteMoveRight;
	std::unique_ptr<animatedSprite> playerSpriteMoveBack;
	std::unique_ptr<animatedSprite> playerSpriteMoveLeft;
	std::unique_ptr<animatedSprite> playerSpriteIdleForward;
	std::unique_ptr<animatedSprite> playerSpriteIdleRight;
	std::unique_ptr<animatedSprite> playerSpriteIdleBack;
	std::unique_ptr<animatedSprite> playerSpriteIdleLeft;

	AnimationMode AnimMode = AnimationMode::IdleBack;
	bool ActivityInFrame=false;
	//Collision box of the dot
	SDL_Rect mBox;
	//The velocity of the dot
	int mVelX, mVelY;

	float x=0;
	float y=0;

	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	//Maximum axis velocity of the dot
	static const int DOT_VEL = 10;
};

