#include "game.h"

game::game()
{
	tiled_map_level = std::make_unique<level>("name");
	player = std::make_unique<Player>();
}

void game::Init()
{
	tiled_map_level.get()->load("island.tmx", g_pFramework->GetRenderer());
	player.get()->Init();
	m_bGameRun = true;
}

void game::Quit()
{

}



void game::Run()
{
	while (m_bGameRun == true)
	{
		ProcessEvents();
		player.get()->Update();
		player.get()->setCamera(camera, tiled_map_level->GetLevelWidth(), tiled_map_level->GetLevelHight());
		g_pFramework->Update();
		g_pFramework->Clear();

		tiled_map_level.get()->drawLayer(g_pFramework->GetRenderer(), camera,0);
		tiled_map_level.get()->drawLayer(g_pFramework->GetRenderer(), camera,1);
		player.get()->Draw(camera);

		tiled_map_level.get()->drawLayer(g_pFramework->GetRenderer(), camera, 2);
		CheckCollisions();

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