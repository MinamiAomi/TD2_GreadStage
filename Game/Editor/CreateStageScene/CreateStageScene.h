#pragma once
#include "Engine/Scene/BaseScene.h"
#include <memory>
#include <vector>
#include <string>
#include "Game/Stage/Stage.h"
#include "Game/DebugCamera.h"
#include "Game/CameraAnimation/CameraAnimation.h"
#include "Game/Player/Player.h"

class GlobalVariables;

class CreateStageScene : public BaseScene {
public:

	void OnInitialize() override;
	void OnUpdate() override;
	void OnFinalize() override;

private: // メンバ変数
	//std::vector<std::shared_ptr<Box>> boxes_;
	std::unique_ptr<Stage> stage_;
	std::shared_ptr<DebugCamera> debugCamera_;
	std::shared_ptr<CameraAnimation> camera_;
	std::shared_ptr<Player> player_;

	GlobalVariables* global_ = nullptr;
	std::vector<std::string> fileName_;
	std::string loadSelectName_;
	char itemName_[256]{};
	int fileNumber_ = 0;

	bool playFlg_ = false;

private:
	void DrawImGui();

};