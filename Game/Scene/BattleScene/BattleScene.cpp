#include "BattleScene.h"

#include "Collision/CollisionManager.h"

void BattleScene::OnInitialize() {
	// 生成
	player_ = std::make_shared<Player>();
	camera_ = std::make_shared<CameraAnimation>();
	stage_ = std::make_shared<Stage>();

	// 初期化
	player_->Initialize();
	camera_->Initialize();
	stage_->Initialize();

	// セット
	player_->SetCamera(camera_);
	camera_->SetTarget(&player_->transform);

}

void BattleScene::OnUpdate() {
	stage_->Update();
	player_->Update();

	CollisionManager::GetInstance()->CheckCollision();
	// カメラの更新
	camera_->Update();
}

void BattleScene::OnFinalize() {

}
