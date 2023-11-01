#pragma once
#include "Engine/Scene/BaseScene.h"

class BattleScene : public BaseScene {
public:

	void OnInitialize() override;
	void OnUpdate() override;
	void OnFinalize() override;

};