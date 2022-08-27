#pragma once
#include "framework.h"
#include <string>

class Sprite
{
public:
	Sprite();
	~Sprite();

	void Load(const std::string sFilname);
	void Load(const std::string sFilname, int NumFrames, int FrameWidth, int FrameHeight);
	void SetPos(float fXPos, float fYPos);
	void Render();
	void ProcessMoving();
	void Update();
	void Render(float fFrameNumber);
	SDL_Rect GetRect() { return m_Rect; }
	float xcount;
	float ycount;
private:
	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pImage;
	SDL_Rect m_Rect;
	SDL_Rect m_FrameRect;
	int m_NumFrames;
	int m_FrameWidth;
	int m_FrameHeight;
	int m_NumFramesX;
};

