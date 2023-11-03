#include "BattleScene.h"

void BattleScene::OnInitialize() {
	// 生成
	player_ = std::make_shared<Player>();
	camera_ = std::make_shared<CameraAnimation>();

	// 初期化
	player_->Initialize();
	camera_->Initialize();

	// セット
	player_->SetCamera(camera_);
	camera_->SetTarget(&player_->transform);

}

void BattleScene::OnUpdate() {
	player_->Update();

	// カメラの更新
	camera_->Update();
}

void BattleScene::OnFinalize() {

}
