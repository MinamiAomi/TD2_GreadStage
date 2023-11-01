#pragma once
#include "Framework/Game.h"

class MasterGame : public Game {
public:

	void OnInitialize() override;
	void OnFinalize() override;

};