#include "TitleScene.h"

#include "Scene/SceneManager.h"
#include "Collision/CollisionManager.h"
#include "Input/Input.h"
#include "Scene/BattleScene/BattleScene.h"
#include "Transition/Transition.h"

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
	stage_->SetPlayerPtr(player_);
	player_->SetCamera(camera_);
	camera_->SetTarget(&player_->transform);

	stage_->Load("StageSelect");
}

void TitleScene::OnUpdate() {
	
	stage_->Update();
	player_->Update();

	player_->PreCollisionUpdate();
	CollisionManager::GetInstance()->CheckCollision();
	player_->PostCollisionUpdate();

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
