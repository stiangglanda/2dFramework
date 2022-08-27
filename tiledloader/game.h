#include "Framework.h"
#include "level.hpp"
#include "Player.h"

class game {
public:
	game();

	void Init();
	void Run();
	void Quit();

private:
	void ProcessEvents();
	void CheckCollisions();
	bool m_bGameRun;
	std::unique_ptr<level> tiled_map_level;
	std::unique_ptr<Player> player;
	SDL_Rect camera = { 0, 0, g_pFramework->GetScreenWidth(), g_pFramework->GetScreenHeight()};
};

