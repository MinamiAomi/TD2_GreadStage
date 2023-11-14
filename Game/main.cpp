#include <Windows.h>

#include "Framework/Engine.h"
#include "Framework/Game.h"
#include "Game/Editor/Editor.h"
#include "Game/MasterGame/MasterGame.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

    Game* game = new Editor;
    Engine::Run(game);
    delete game;
    return 0;
}