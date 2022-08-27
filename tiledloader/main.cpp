#include "Framework.h"
#include "game.h"

int main(int argc, char* argv[]) {
    if (g_pFramework->Init(800, 600, 16, false) == false)
        return (0);

    game Game;

    Game.Init();

    Game.Run();

    Game.Quit();

    g_pFramework->Quit();
    g_pFramework->Del();

    return (0);

}