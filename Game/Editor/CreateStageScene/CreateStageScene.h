#pragma once
#include "Engine/Scene/BaseScene.h"
#include <memory>
#include <vector>
#include <string>
#include "Game/Stage/Box/Box.h"
#include "Game/CameraAnimation/CameraAnimation.h"

class GlobalVariables;

class CreateStageScene : public BaseScene {
public:

	void OnInitialize() override;
	void OnUpdate() override;
	void OnFinalize() override;

private: // メンバ変数
	std::vector<std::shared_ptr<Box>> boxes_;
	std::shared_ptr<CameraAnimation> camera_;
	GlobalVariables* global_ = nullptr;
	const std::string groupName = "Stage";
private:
	void DrawImGui();

};