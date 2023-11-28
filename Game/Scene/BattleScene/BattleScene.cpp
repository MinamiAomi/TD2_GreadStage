#include "BattleScene.h"

#include "Collision/CollisionManager.h"
#include "Graphics/RenderManager.h"

#include "Graphics/ImGuiManager.h"
#include "Engine/Scene/SceneManager.h"
#include "Game/Scene/TitleScene/TitleScene.h"
#include "Transition/Transition.h"
#include "Engine/Input/Input.h"

void BattleScene::OnInitialize() {
	// 生成
	player_ = std::make_shared<Player>();
	camera_ = std::make_shared<CameraAnimation>();
	stage_ = std::make_shared<Stage>();
	pause_ = std::make_unique<GamePause>();

	// 初期化
	player_->Initialize();
	camera_->Initialize();
	stage_->Initialize();
	pause_->Initialize();

	// セット
	stage_->SetPlayerPtr(player_);
	player_->SetCamera(camera_);
	camera_->SetTarget(&player_->transform);

	//stage_->Load("Stage3");
	auto i = Transition::GetInstance();
	stage_->Load(i->GetStageId());
}

void BattleScene::OnUpdate() {
	auto input = Input::GetInstance();
	auto& xInput = input->GetXInputState();
	auto& preXInput = input->GetPreXInputState();

	// ポーズ : Back(左のなんか)
	if (xInput.Gamepad.wButtons & XINPUT_GAMEPAD_BACK && !(preXInput.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)) {
		isPaused_ = !isPaused_;
	}

	isPaused_ ? PauseUpdate() : NormalUpdate();

}

void BattleScene::OnFinalize() {

}

void BattleScene::NormalUpdate() {
	stage_->Update();
	player_->Update();

	player_->PreCollisionUpdate();
	CollisionManager::GetInstance()->CheckCollision();
	player_->PostCollisionUpdate();
	// カメラの更新
	camera_->Update();

	//float playerToCameraDistance = (player_->transform.worldMatrix.GetTranslate() - camera_->transform.worldMatrix.GetTranslate()).Length();
	//RenderManager::GetInstance()->GetModelRenderer().DitheringRange(playerToCameraDistance - 1.0f);
#ifdef _DEBUG


	ImGui::Begin("test");
	static float dither = 0.0f;
	static float blur = 0.0f;
	static bool useBlur = false;
	ImGui::DragFloat("Dither", &dither, 0.01f);
	ImGui::DragFloat("Blur", &blur, 0.01f, 0.0f, 1.0f);
	ImGui::Checkbox("Use GaussianBlur", &useBlur);

	ImGui::End();

	//dither = playerToCameraDistance - 5.0f;
	RenderManager::GetInstance()->GetModelRenderer().DitheringRange(dither);
	RenderManager::GetInstance()->GetGaussianBlur().UpdateWeightTable(blur);
	RenderManager::GetInstance()->UseGaussianBlur(useBlur);

#endif // _DEBUG

	if (player_->GetCleared()) {
		Transition::GetInstance()->SetComeToStage();
	}
	auto trans = Transition::GetInstance();
	if (trans->Update()) {
		// シーンのシングルトンの取得
		SceneManager* sceneManager = SceneManager::GetInstance();
		// シーンの設定
		sceneManager->ChangeScene<TitleScene>();
	}
}

void BattleScene::PauseUpdate() {
	pause_->Update();
}
