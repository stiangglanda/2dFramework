#include "game.h"
#include <algorithm>

game::game()
{
	tiled_map_level = std::make_unique<level>("name");
	player = std::make_unique<Player>();
}

void game::Init()
{
	tiled_map_level.get()->load("island.tmx", g_pFramework->GetRenderer());
	player.get()->Init();

	SDL_Color textColor = { 0, 0, 0 };
	if (!Widget.loadFromRenderedText("The quick brown fox jumps over the lazy dog", textColor))
	{
		printf("Failed to render text texture!\n");
	}
	m_bGameRun = true;
}

void game::Quit()
{
	tiled_map_level.release();
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
		//player.get()->Update(&collis);//might need camera for collision
		player.get()->Update(tiled_map_level.get()->GetCollisionLayer());//might need camera for collision
		player.get()->setCamera(camera, tiled_map_level->GetLevelWidth(), tiled_map_level->GetLevelHight());
		//std::sort(tiled_map_level.get()->GetTilesByLayer(1)->begin(), tiled_map_level.get()->GetTilesByLayer(1)->begin());
		g_pFramework->Update();
		g_pFramework->Clear();

		tiled_map_level.get()->drawLayer(g_pFramework->GetRenderer(), camera,0);
		tiled_map_level.get()->drawLayer(g_pFramework->GetRenderer(), camera,1);
		player.get()->Draw(camera);
		tiled_map_level.get()->drawLayer(g_pFramework->GetRenderer(), camera, 2);

		
		for (int i =0;i< tiled_map_level.get()->GetCollisionLayer()->size();i++)
		{
			g_pFramework->RenderRect(tiled_map_level.get()->GetCollisionLayer()->at(i), camera);
		}
		CheckCollisions();

		//draw UI
		Widget.render((g_pFramework->GetScreenWidth() - Widget.getWidth()) / 2, (g_pFramework->GetScreenHeight() - Widget.getHeight()) / 2);

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
	}
}

void game::CheckCollisions()
{
	
}