#pragma once
#include "Engine/Scene/BaseScene.h"
#include <memory>

#include "Game/Player/Player.h"
#include "Game/CameraAnimation/CameraAnimation.h"
#include "Game/Stage/Stage.h"

class BattleScene : public BaseScene {
public:

	void OnInitialize() override;
	void OnUpdate() override;
	void OnFinalize() override;

private: // メンバ変数
	std::shared_ptr<Player> player_;
	std::shared_ptr<CameraAnimation> camera_;
	std::shared_ptr<Stage> stage_;

};