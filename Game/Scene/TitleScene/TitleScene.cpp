#include "TitleScene.h"

#include "Scene/SceneManager.h"
#include "Collision/CollisionManager.h"
#include "Input/Input.h"
#include "Scene/BattleScene/BattleScene.h"

void TitleScene::OnInitialize() {
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

void TitleScene::OnUpdate() {
	auto input = Input::GetInstance();
	if (input->IsKeyPressed(DIK_0)) {
		// シーンのシングルトンの取得
		SceneManager* sceneManager = SceneManager::GetInstance();
		// シーンの設定
		sceneManager->ChangeScene<BattleScene>();
	}
	
	stage_->Update();
	player_->Update();

	CollisionManager::GetInstance()->CheckCollision();
	// カメラの更新
	camera_->Update();
}

void TitleScene::OnFinalize() {

}
