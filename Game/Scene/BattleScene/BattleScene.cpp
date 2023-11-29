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
	pause_->Initialize(1);

	// セット
	stage_->SetPlayerPtr(player_);
	player_->SetCamera(camera_);
	camera_->SetTarget(&player_->transform);

	//stage_->Load("Stage3");
	auto i = Transition::GetInstance();
	stage_->Load(i->GetStageId());


	CameraAnimation::EaseParameter parm;
	switch (i->getNum()) {
	case 1:
		parm.first  = { {0.0f,-9.5f,42.0f},{0.0f,180.0f},0.05f };
		parm.second = { {0.0f,-9.5f,42.0f},{0.0f,180.0f},0.02f };
		parm.third  = { {-17.0f,-9.5f,27.0f},{-6.0f,140.0f},0.02f };
		parm.fourth = { {-30.0f,5.0f,-10.0f},{20.0f,56.0f},0.02f };
		parm.fifth  = { {0.0f,6.0f,-16.0f},{19.0f,0.0f},0.0f };
		break;
	case 2:
		parm.first  = { {0.0f,15.0f,30.0f},{42.0f,180.0f},0.05f };
		parm.second = { {0.0f,15.0f,30.0f},{42.0f,180.0f},0.02f };
		parm.third  = { {22.0f,-2.3f,30.0f},{18.5f,225.0f},0.02f };
		parm.fourth = { {-21.0f,-19.0f,9.0f},{-12.0f,80.0f},0.01f };
		parm.fifth  = { {-13.0f,2.5f,-18.0f},{24.0f,33.0f},0.0f };
		break;
	case 3:
		parm.first  = { {0.0f,21.0f,60.0f},{33.0f,-180.0f},0.05f };
		parm.second = { {0.0f,21.0f,60.0f},{33.0f,-180.0f},0.02f };
		parm.third  = { {24.0f,14.0f,51.0f},{21.0f,-119.0f},0.01f };
		parm.fourth = { {24.0f,10.0f,3.0f},{17.0f,-92.0f},0.02f };
		parm.fifth  = { {4.0f,10.0f,-18.0f},{16.5f,-7.0f},0.0f };
		break;
	case 4:
		parm.first  = { {0.0f,0.0f,35.0f},{0.0f,180.0f},0.05f };
		parm.second = { {0.0f,0.0f,35.0f},{0.0f,180.0f},0.02f };
		parm.third  = { {-23.0f,-14.0f,28.0f},{6.0f,120.0f},0.01f };
		parm.fourth = { {-24.0f,-3.0f,-14.0f},{-7.6f,41.0f},0.02f };
		parm.fifth  = { {-6.5f,9.0f,-15.0f},{12.0f,7.0f},0.0f };
		break;
	case 5:
		parm.first  = { {-32.0f,5.0f,-45.0f},{0.0f,0.0f},0.03f };
		parm.second = { {-32.0f,5.0f,-45.0f},{-16.0f,0.0f},0.01f };
		parm.third  = { {-26.0f,40.0f,-27.0f},{36.0f,22.0f},0.01f };
		parm.fourth = { {0.7f,35.0f,14.9f},{66.0f,122.0f},0.01f };
		parm.fifth  = { {18.0f,3.0f,-5.0f},{-7.0f,300.0f},0.0f };
		break;
	}
	camera_->SetEaseParameter(parm);
	stage_->SetModel(i->GetStageId());
}

void BattleScene::OnUpdate() {

#ifdef _DEBUG
	//	ImGui のフレームに一つ目の ImGui のウィンドウを描く
	ImGui::Begin("Control panel");
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();
#endif // _DEBUG

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
	auto trans = Transition::GetInstance();
	pause_->SetDraw(false);

	stage_->Update();
	if (!trans->GetIsTransition() && !camera_->GetStageMove()) {
		player_->Update();
	}

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
