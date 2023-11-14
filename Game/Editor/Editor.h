#pragma once
#include "Framework/Game.h"

class Editor : public Game {
public:

	void OnInitialize() override;
	void OnFinalize() override;

};