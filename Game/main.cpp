#include <Windows.h>

#include "Framework/Engine.h"
#include "Framework/Game.h"
#include "MasterGame/MasterGame.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

    Game* game = new MasterGame;
    Engine::Run(game);
    delete game;
    return 0;
}