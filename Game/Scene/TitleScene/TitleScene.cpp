#include "TitleScene.h"

#include "Scene/SceneManager.h"
#include "Collision/CollisionManager.h"
#include "Input/Input.h"
#include "Scene/BattleScene/BattleScene.h"
#include "Transition/Transition.h"
#include <algorithm>

decltype(TitleScene::leavingTime_) TitleScene::leavingTime_;

void TitleScene::OnInitialize() {
	// 生成
	player_ = std::make_shared<Player>();
	camera_ = std::make_shared<CameraAnimation>();
	stage_ = std::make_shared<Stage>();
	titleText_ = std::make_shared<TitleText>();

	camera_->SetTitleScene(true);

	// 初期化
	player_->Initialize();
	camera_->Initialize();
	stage_->Initialize();
	titleText_->Initialize();

	// セット
	stage_->SetPlayerPtr(player_);
	player_->SetCamera(camera_);
	camera_->SetTarget(&player_->transform);

	stage_->Load("StageSelect");

	leavingTime_ = 300u;
}

void TitleScene::OnUpdate() {

	CheckInput() ? leavingTime_ = 0u : leavingTime_++;
	
	titleText_->DrawImGui();

	stage_->Update();
	titleText_->Update();
	player_->Update();

	player_->PreCollisionUpdate();
	CollisionManager::GetInstance()->CheckCollision();
	player_->PostCollisionUpdate();

	// 最大放置時間
	const uint32_t kMaxTime = 300u;
	leavingTime_ = std::clamp(leavingTime_, 0u, kMaxTime);
	if (leavingTime_ >= kMaxTime) {
		camera_->SetTitleMove(true);
	}
	else {
		camera_->SetTitleMove(false);
	}

	// カメラの更新
	camera_->Update();

	auto trans = Transition::GetInstance();
	if (trans->Update()) {
		// シーンのシングルトンの取得
		SceneManager* sceneManager = SceneManager::GetInstance();
		// シーンの設定
		sceneManager->ChangeScene<BattleScene>();
	}

}

void TitleScene::OnFinalize() {

}

bool TitleScene::CheckInput() {
	auto input = Input::GetInstance();
	auto& xInput = input->GetXInputState();
	auto& preXInput = input->GetPreXInputState();

	// 右スティック : カメラ
	if (std::abs(xInput.Gamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		return true;
	}
	if (std::abs(xInput.Gamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		return true;
	}
	// 左スティック : 移動
	if (std::abs(xInput.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		return true;
	}
	if (std::abs(xInput.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		return true;
	}
	// ジャンプ : A
	if (xInput.Gamepad.wButtons & XINPUT_GAMEPAD_A && !(preXInput.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
		return true;
	}
	// ポーズ : Back(左のなんか)
	if (xInput.Gamepad.wButtons & XINPUT_GAMEPAD_BACK && !(preXInput.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)) {
		return true;
	}
	return false;
}
