#include "BattleScene.h"

#include "Collision/CollisionManager.h"
#include "Graphics/RenderManager.h"

#include "Graphics/ImGuiManager.h"

void BattleScene::OnInitialize() {
	// 生成
	player_ = std::make_shared<Player>();
	camera_ = std::make_shared<CameraAnimation>();
	stage_ = std::make_shared<Stage>();

	// 初期化
	player_->Initialize();
	camera_->Initialize();
	stage_->Initialize();
	stage_->Load("MasterTestStage");

	// セット
	player_->SetCamera(camera_);
	camera_->SetTarget(&player_->transform);

}

void BattleScene::OnUpdate() {
	stage_->Update();
	player_->Update();

	player_->PreCollisionUpdate();
	CollisionManager::GetInstance()->CheckCollision();
	player_->PostCollisionUpdate();
	// カメラの更新
	camera_->Update();

	//float playerToCameraDistance = (player_->transform.worldMatrix.GetTranslate() - camera_->transform.worldMatrix.GetTranslate()).Length();
	//RenderManager::GetInstance()->GetModelRenderer().DitheringRange(playerToCameraDistance - 1.0f);

	ImGui::Begin("test");
	static float dither = 0.0f;
	ImGui::DragFloat("Dither", &dither, 0.01f);
	ImGui::End();
	//dither = playerToCameraDistance - 5.0f;
	RenderManager::GetInstance()->GetModelRenderer().DitheringRange(dither);
}

void BattleScene::OnFinalize() {

}
