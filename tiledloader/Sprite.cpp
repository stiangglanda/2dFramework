#include "Sprite.h"
#include <iostream>

Sprite::Sprite()
{
	m_pRenderer = g_pFramework->GetRenderer();
	xcount = 0;
	ycount = 0;
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(m_pImage);
}

void Sprite::Load(const std::string sFilename)
{
	SDL_Surface* pTemp = SDL_LoadBMP(sFilename.c_str());
	if (pTemp == NULL)
	{
		std::cout << "Fehler beim Laden von: " << sFilename.c_str();
		std::cout << std::endl;
		std::cout << "Fehlermeldung: " << SDL_GetError() << std::endl;

		g_pFramework->Quit();
		exit(1);
	}
	SDL_SetColorKey(pTemp, SDL_TRUE, SDL_MapRGB(pTemp->format, 255, 0, 255));
	m_pImage = SDL_CreateTextureFromSurface(m_pRenderer, pTemp);
	m_Rect.x = 0;
	m_Rect.y = 0;
	m_Rect.w = pTemp->w;
	m_Rect.h = pTemp->h;

	SDL_FreeSurface(pTemp);
}

void Sprite::Load(const std::string sFilename, int NumFrames, int FrameWidth, int FrameHeight)
{
	Load(sFilename);
	m_NumFrames = NumFrames;
	m_FrameWidth = FrameWidth;
	m_FrameHeight = FrameHeight;
	m_FrameRect.w = FrameWidth;
	m_FrameRect.h = FrameHeight;
	m_NumFramesX = m_Rect.w / m_FrameWidth;

	m_Rect.w = FrameWidth;
	m_Rect.h = FrameHeight;
}

void Sprite::SetPos(float fXPos, float fYPos)
{
	m_Rect.x = static_cast<int> (fXPos);
	m_Rect.y = static_cast<int> (fYPos);
}

void Sprite::Render()
{
	SDL_RenderCopy(m_pRenderer, m_pImage, NULL, &m_Rect);
}

void Sprite::Render(float fFrameNumber)
{
	int Column = static_cast<int>(fFrameNumber) % m_NumFramesX;
	int Row = static_cast<int>(fFrameNumber) / m_NumFramesX;
	m_FrameRect.x = Column * m_FrameWidth;
	m_FrameRect.y = Row * m_FrameHeight;
	SDL_RenderCopy(m_pRenderer, m_pImage, &m_FrameRect, &m_Rect);
}

void Sprite::Update()
{
	ProcessMoving();
}


void Sprite::ProcessMoving()
{
	if (g_pFramework->KeyDown(SDL_SCANCODE_LEFT))
	{
		xcount += 300.0f * Timer::Get()->GetElapsed();
		SetPos(static_cast<int>(xcount), m_Rect.y);
	}

	if (g_pFramework->KeyDown(SDL_SCANCODE_RIGHT))
	{
		xcount -= 300.0f * Timer::Get()->GetElapsed();
		SetPos(static_cast<int>(xcount), m_Rect.y);
	}

	if (g_pFramework->KeyDown(SDL_SCANCODE_UP))
	{
		ycount += 300.0f * Timer::Get()->GetElapsed();
		SetPos(m_Rect.x, static_cast<int>(ycount));
	}

	if (g_pFramework->KeyDown(SDL_SCANCODE_DOWN))
	{
		ycount -= 300.0f * Timer::Get()->GetElapsed();
		SetPos(m_Rect.x, static_cast<int>(ycount));
	}
}