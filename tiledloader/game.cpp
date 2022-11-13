#include "game.h"
#include <algorithm>

struct ApplicationData {
	bool show_text = true;
	Rml::String animal = "dog";
} my_data;

game::game()
{
	tiled_map_level = std::make_unique<level>("name");
	player = std::make_unique<Player>();

	render_interface = std::make_unique<RenderInterface_SDL>(g_pFramework->GetRenderer());
	system_interface = std::make_unique<SystemInterface_SDL>();


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

	Rml::SetRenderInterface(render_interface.get());
	Rml::SetSystemInterface(system_interface.get());

	Rml::Initialise();

	context = Rml::CreateContext("main", Rml::Vector2i(g_pFramework->GetScreenWidth(), g_pFramework->GetScreenHeight()));

	Rml::LoadFontFace("LatoLatin-Regular.ttf");
	Rml::LoadFontFace("NotoEmoji-Regular.ttf", true);

	if (Rml::DataModelConstructor constructor = context->CreateDataModel("animals"))
	{
		constructor.Bind("show_text", &my_data.show_text);
		constructor.Bind("animal", &my_data.animal);
	}

	Rml::ElementDocument* document = context->LoadDocument("hello_world.rml");
	document->Show();

	Rml::Element* element = document->GetElementById("world");
	//element->SetInnerRML(reinterpret_cast<const char*>(u8"🌍"));
	element->SetProperty("font-size", "1.5em");

}

void game::Quit()
{
	tiled_map_level.release();
	Widget.free();
	player.release();
	Rml::Shutdown();
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

		context->Update();

		// Render the user interface. All geometry and other rendering commands are now
		// submitted through the render interface.
		context->Render();

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