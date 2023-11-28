#pragma once
#include "Engine/Scene/BaseScene.h"
#include <memory>

#include "Game/Player/Player.h"
#include "Game/CameraAnimation/CameraAnimation.h"
#include "Game/Stage/Stage.h"
#include "Graphics/LightManager.h"

class BattleScene : public BaseScene {
public:

	void OnInitialize() override;
	void OnUpdate() override;
	void OnFinalize() override;

private: // メンバ変数
	std::shared_ptr<Player> player_;
	std::shared_ptr<CameraAnimation> camera_;
	std::shared_ptr<Stage> stage_;

	bool isPaused_ = false; // true : ポーズ中/false : 通常

private:
	void NormalUpdate(); // 通常のゲームシーン用更新
	void PauseUpdate(); // ポーズ用更新

};