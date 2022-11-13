#include "Framework.h"
#include "level.hpp"
#include "Player.h"
#include "BaseWidget.h"
#include <RmlUi/Core.h>

#include "RenderInterface_SDL.h"
#include "SystemInterface_SDL.h"



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
	LTexture Widget;
	SDL_Rect camera = { 0, 0, g_pFramework->GetScreenWidth(), g_pFramework->GetScreenHeight()};
	std::unique_ptr<RenderInterface_SDL> render_interface;
	std::unique_ptr<SystemInterface_SDL> system_interface;
	Rml::Context* context;
};

