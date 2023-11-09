#include "Framework.h"
#include "level.hpp"
#include "Player.h"
#include "BaseWidget.h"
#include "GUISystem.h"

#include "pathfinding.h"


class game
{
public:
	game();

	void Init();
	void Run();
	void Quit();

private:
	void ProcessEvents();
	void CheckCollisions();
	bool m_bGameRun;
	std::shared_ptr<level> tiled_map_level;
	std::unique_ptr<Player> player;
	std::unique_ptr<pathfinding> mPathfinding;
	LTexture Widget;
	LTexture Image;
	SDL_Rect camera = {0, 0, g_pFramework->GetScreenWidth(), g_pFramework->GetScreenHeight()};
	GUISystem gui;
};
