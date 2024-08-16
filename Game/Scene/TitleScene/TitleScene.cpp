#include "TitleScene.h"

#include "Scene/SceneManager.h"
#include "Collision/CollisionManager.h"
#include "Input/Input.h"
#include "Scene/BattleScene/BattleScene.h"
#include "Transition/Transition.h"
#include <algorithm>

#include "Graphics/ImGuiManager.h"
#include "Graphics/ResourceManager.h"

decltype(TitleScene::leavingTime_) TitleScene::leavingTime_ = 300u;

void TitleScene::OnInitialize() {
	// 生成
	player_ = std::make_shared<Player>();
	camera_ = std::make_shared<CameraAnimation>();
	stage_ = std::make_shared<Stage>();
	titleText_ = std::make_shared<TitleText>();
	pause_ = std::make_unique<GamePause>();
	titleController_ = std::make_unique<Sprite>();

	// 初期化
#ifdef _DEBUG
	debugCamera_.Initialize();
#endif // _DEBUG
	player_->Initialize();
	camera_->Initialize();
	stage_->Initialize();
	titleText_->Initialize();
	pause_->Initialize(0);

	titleController_->SetTexture(ResourceManager::GetInstance()->FindTexture("Controller"));
	titleController_->SetTexcoordRect({ 0.0f,0.0f }, { titleController_->GetTexture()->GetWidth(), titleController_->GetTexture()->GetHeight() });
	titleController_->SetScale({ titleController_->GetTexture()->GetWidth() / 4, titleController_->GetTexture()->GetHeight() / 4 });
	titleController_->SetPosition({ 1100.0f,90.0f });
	titleController_->SetDrawOrder(2u);
	titleController_->SetIsActive(true);


	// セット
	stage_->SetPlayerPtr(player_);
	player_->SetCamera(camera_);
	camera_->SetTarget(&player_->transform);

	stage_->Load("StageSelect");
	stage_->SetModel("StageSelect");

	/*operation_ = std::make_unique<Sprite>();
	operation_->SetTexture(ResourceManager::GetInstance()->FindTexture("Operation"));
	operation_->SetTexcoordRect({ 0.0f,0.0f }, { operation_->GetTexture()->GetWidth(), operation_->GetTexture()->GetHeight() });
	operation_->SetScale(Vector2(640.0f,360.0f));
	operation_->SetPosition(Vector2(960.0f,180.0f));
	operation_->SetDrawOrder(1);
	operation_->SetIsActive(true);*/
}

void TitleScene::OnUpdate() {
#ifdef _DEBUG
	//	ImGui のフレームに一つ目の ImGui のウィンドウを描く
	ImGui::Begin("Control panel");
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();
#endif // _DEBUG

	auto input = Input::GetInstance();
	auto& xInput = input->GetXInputState();
	auto& preXInput = input->GetPreXInputState();

	CheckInput() ? leavingTime_ = 0u : leavingTime_++;

	// ポーズ : Back(左のなんか)
	if (xInput.Gamepad.wButtons & XINPUT_GAMEPAD_BACK && !(preXInput.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)) {
		isPaused_ = !isPaused_;
	}

	isPaused_ ? PauseUpdate() : NormalUpdate();

	ImGui::Begin("test");
	if (ImGui::Checkbox("Use DebugCamera", &useDebugCamera_)) {
		if (useDebugCamera_) {
			debugCamera_.GetTransform()->translate = camera_->GetCamera()->GetPosition();
			debugCamera_.GetTransform()->rotate = camera_->GetCamera()->GetRotate();
			debugCamera_.Update();
			debugCamera_.SetCamera();
		}
		else {
			camera_->SetCamera();
		}
	}
	ImGui::End();

}

void TitleScene::NormalUpdate() {
	auto trans = Transition::GetInstance();
	pause_->SetDraw(false);
	
	titleText_->DrawImGui();

	stage_->Update();
	titleText_->Update();
	if (!trans->GetIsTransition()) {
		player_->Update();
	}

	player_->PreCollisionUpdate();
	CollisionManager::GetInstance()->CheckCollision();
	player_->PostCollisionUpdate();

	// 最大放置時間
	const uint32_t kMaxTime = 300u;
	leavingTime_ = std::clamp(leavingTime_, 0u, kMaxTime);
	if (leavingTime_ >= kMaxTime) {
		camera_->SetTitleMove(true);
		CameraAnimation::nowTitle_ = true;
		pause_->PoseDraw(false);
		titleController_->SetIsActive(true);
	}
	else {
		camera_->SetTitleMove(false);
		CameraAnimation::nowTitle_ = true;
		pause_->PoseDraw(true);
		titleController_->SetIsActive(false);
	}

	// カメラの更新
	camera_->TitleUpdate();
#ifdef _DEBUG
	if (useDebugCamera_) {
		debugCamera_.Update();
	}
#endif // _DEBUG

	if (trans->Update()) {
		// シーンのシングルトンの取得
		SceneManager* sceneManager = SceneManager::GetInstance();
		// シーンの設定
		sceneManager->ChangeScene<BattleScene>();
	}
}

void TitleScene::PauseUpdate() {
	pause_->Update();
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
