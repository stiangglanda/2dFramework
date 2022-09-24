#pragma once

#include <iostream>
#include "TSingleton.h"
#include "Timer.h"
#include <vector>

using namespace std;

#define g_pFramework Framework::Get()

class Framework :public TSingleton<Framework>
{
public:
	Framework();
	bool Init(int ScreenWidth, int ScreenHeight, int ColorDepth, bool bFullscreen);
	void Quit();
	void Update();
	void Clear();
	void Render();
	bool KeyDown(int Key_ID);
	bool checkCollision(SDL_Rect a, SDL_Rect b);
	SDL_Renderer* GetRenderer() { return m_pRenderer; }
	int GetScreenWidth() { return mWidth; }
	int GetScreenHeight() { return mHight; }
	bool touchesWall(SDL_Rect box, const std::vector<SDL_Rect>* collisionLayer);
	void RenderRect(SDL_Rect outlineRect, SDL_Rect camera);

private:
	const Uint8* m_pKeystate;
	int mHight;
	int mWidth;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
};

