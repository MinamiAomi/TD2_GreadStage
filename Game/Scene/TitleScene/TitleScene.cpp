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
	titleText_ = std::make_shared<TitleText>();

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
}

void TitleScene::OnUpdate() {
	
	titleText_->DrawImGui();

	stage_->Update();
	titleText_->Update();
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
