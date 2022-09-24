#include "Framework.h"
#include <SDL2/SDL_image.h>

Framework::Framework()
{
	m_pWindow = NULL;
	m_pRenderer = NULL;
}

bool Framework::Init(int ScreenWidth, int ScreenHeight, int ColorDepth, bool bFullscreen)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
	{
		cout << "SDL could not initialize!" << endl;
		cout << "Error: " << SDL_GetError() << endl;

		Quit();
		return false;
	}

	Uint32 WindowFlags = 0;

	if (bFullscreen == true)
	{
		WindowFlags = SDL_WINDOW_FULLSCREEN;
	}
	else
	{
		WindowFlags = SDL_WINDOW_SHOWN;
	}

	m_pWindow = SDL_CreateWindow("nudyDOG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, WindowFlags);

	if (m_pWindow == NULL)
	{
		cout << "SDL-Window could not be Created!" << endl;
		cout << "Error: " << SDL_GetError() << endl;

		Quit();
		return false;
	}

	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
	if (m_pRenderer == NULL)
	{
		cout << "SDL-Renderer could not be Created!" << endl;
		cout << "Error: " << SDL_GetError() << endl;

		Quit();
		return false;
	}
	else
	{
		//Initialize renderer color
		SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

		//Initialize PNG loading
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		}
	}

	mWidth = ScreenWidth;
	mHight = ScreenHeight;

	m_pKeystate = SDL_GetKeyboardState(NULL);

	return true;
}

void Framework::Quit()
{
	if (m_pRenderer != NULL)
	{
		SDL_DestroyRenderer(m_pRenderer);
	}

	if (m_pWindow != NULL)
	{
		SDL_DestroyWindow(m_pWindow);
	}

	IMG_Quit();
	SDL_Quit();
}

bool Framework::checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

void Framework::RenderRect(SDL_Rect outlineRect, SDL_Rect camera)
{
	//Render green outlined quad
	outlineRect.x -= camera.x;
	outlineRect.y -= camera.y;
	SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawRect(m_pRenderer, &outlineRect);
}

bool Framework::touchesWall(SDL_Rect box, const std::vector<SDL_Rect>* collisionLayer)
{
	//Go through the tiles
	for (int i = 0; i < collisionLayer->size(); ++i)
	{
			//If the collision box touches the wall tile
		if (checkCollision(box, collisionLayer->at(i)))
		{
			return true;
		}
	}

	//If no wall tiles were touched
	return false;
}

void Framework::Update()
{
	Timer::Get()->Update();//TODO maybe!!!!!!!!!!!!!! //g_pTimer->Update();

	SDL_PumpEvents();
}

bool Framework::KeyDown(int Key_ID)
{
	return (m_pKeystate[Key_ID] ? true : false);
}

void Framework::Clear()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer);
}

void Framework::Render()
{
	SDL_RenderPresent(m_pRenderer);
}