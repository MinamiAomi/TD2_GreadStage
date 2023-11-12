#pragma once
#include "Engine/Scene/BaseScene.h"
#include <memory>
#include <vector>
#include "Game/Stage/Box/Box.h"
#include "Game/CameraAnimation/CameraAnimation.h"

class CreateStageScene : public BaseScene {
public:

	void OnInitialize() override;
	void OnUpdate() override;
	void OnFinalize() override;

private: // メンバ変数
	std::vector<std::shared_ptr<Box>> boxes_;
	std::shared_ptr<CameraAnimation> camera_;
private:
	void DrawImGui();

};