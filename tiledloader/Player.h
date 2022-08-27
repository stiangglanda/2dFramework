#pragma once
#include "Sprite.h"
#include "LTexture.h"
#include "animatedSprite.h"

class Player
{
public:
	void Init();
	void Draw(SDL_Rect& camera);
	void Update();
	void Quit();
	void setCamera(SDL_Rect& camera, float levelWidth, float levelHight);
private:
	std::unique_ptr<animatedSprite> playerSprite;
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

