#pragma once
#include "Engine/Scene/BaseScene.h"
#include <memory>

#include "Game/CameraAnimation/CameraAnimation.h"

class SelectScene : public BaseScene {
public:

	void OnInitialize() override;
	void OnUpdate() override;
	void OnFinalize() override;

private: // メンバ変数
	std::shared_ptr<CameraAnimation> camera_;

};