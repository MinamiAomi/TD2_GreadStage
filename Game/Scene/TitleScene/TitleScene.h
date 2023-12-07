#pragma once
#include "Engine/Scene/BaseScene.h"
#include <memory>

#include "Game/Player/Player.h"
#include "Game/CameraAnimation/CameraAnimation.h"
#include "Game/Stage/Stage.h"
#include "Game/Stage/TitleText.h"
#include "Game/GamePause/GamePause.h"

class TitleScene : public BaseScene {
public:

	void OnInitialize() override;
	void OnUpdate() override;
	void OnFinalize() override;

private: // メンバ変数
	std::shared_ptr<Player> player_;
	std::shared_ptr<CameraAnimation> camera_;
	std::shared_ptr<Stage> stage_;
	std::shared_ptr<TitleText> titleText_;
	std::unique_ptr<GamePause> pause_;
	bool isPaused_ = false; // true : ポーズ中/false : 通常

	//std::unique_ptr<Sprite> operation_;

private:
	void NormalUpdate();
	void PauseUpdate();
	bool CheckInput();
public:
	// 放置時間
	static uint32_t leavingTime_;


};