#include "game.h"
#include <algorithm>

game::game()
{
	tiled_map_level = std::make_shared<level>("name");
	player = std::make_unique<Player>();
}

static void buttonOnClick()
{
	std::cout << "onClick";
}

void game::Init()
{
	tiled_map_level->load("island.tmx", g_pFramework->GetRenderer());
	mPathfinding = std::make_unique<pathfinding>(tiled_map_level);
	player->Init();

	Image.loadFromFile("button.png");

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 120;
	rect.h = 80;

	SDL_Color textColor = {255, 0, 0};

	gui.CreateLabel("someName", rect, "Fox", textColor);

	rect.x = 120;
	rect.y = 70;
	gui.CreateButton("someName2", rect, "Fox2", textColor, buttonOnClick);

	rect.x = 320;
	rect.y = 40;
	gui.CreateProgress("someName2", rect, textColor, 0.5f);


	rect.x = 320;
	rect.y = 200;
	gui.CreateImage("someName2", rect, &Image);

	if (!Widget.loadFromRenderedText("The quick brown fox jumps over the lazy dog", textColor))
	{
		printf("Failed to render text texture!\n");
	}
	m_bGameRun = true;
}

void game::Quit()
{
	Widget.free();
	player.release();
}


void game::Run()
{
	while (m_bGameRun == true)
	{
		SDL_Rect rect;
		rect.x = 500;
		rect.y = 500;
		rect.w = 50;
		rect.h = 50;
		std::vector<SDL_Rect> collis;
		collis.push_back(rect);
		ProcessEvents();
		player->Update(tiled_map_level->GetCollisionLayer()); //might need camera for collision
		player->setCamera(camera, tiled_map_level->GetLevelWidth(), tiled_map_level->GetLevelHight());
		g_pFramework->Update();
		g_pFramework->Clear();

		tiled_map_level->drawLayer(g_pFramework->GetRenderer(), camera, 0);
		tiled_map_level->drawLayer(g_pFramework->GetRenderer(), camera, 1);
		player->Draw(camera);
		tiled_map_level->drawLayer(g_pFramework->GetRenderer(), camera, 2);


		for (int i = 0; i < tiled_map_level->GetCollisionLayer()->size(); i++)
		{
			g_pFramework->RenderRect(tiled_map_level->GetCollisionLayer()->at(i), camera);
		}
		CheckCollisions();

		//draw UI
		gui.Render();

		mPathfinding->SetEnd(player->GetX() / tiled_map_level->GetTileWidth(),
		                     player->GetY() / tiled_map_level->GetTileWidth());
		mPathfinding->Solve_AStar();

		mPathfinding->debugDraw(g_pFramework->GetRenderer(), camera);


		g_pFramework->Render();
	}
}

void game::ProcessEvents()
{
	SDL_Event Event;

	if (SDL_PollEvent(&Event))
	{
		switch (Event.type)
		{
		case (SDL_QUIT):
			{
				m_bGameRun = false;
				break;
			}
		case (SDL_KEYDOWN):
			{
				switch (Event.key.keysym.sym)
				{
				case (SDLK_ESCAPE):
					{
						m_bGameRun = false;
						break;
					}
				}
				break;
			}
		}

		gui.Update(0);
		gui.handleEvent(&Event);
	}
}

void game::CheckCollisions()
{
}
