#include "GamePause.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/ImGuiManager.h"
#include "Input/Input.h"
#include "Easing/Easing.h"
#include <algorithm>

#include "Transition/Transition.h"
#include "Scene/SceneManager.h"
#include "Scene/TitleScene/TitleScene.h"
#include "Scene/BattleScene/BattleScene.h"

void GamePause::Initialize() {
	
	textureParam_[StageSelect] = { "StageSelect",{900.0f,256.0f},0.0f,{425.0f,73.0f},2u,Vector4::one,true };
	textureParam_[Restart] = { "Restart",{900.0f,256.0f},0.0f,{325.0f,175.0f},2u,Vector4::one,true };
	textureParam_[Pose] = { "Pose",{900.0f,256.0f},0.0f,{645.0f,623.0f},2u,Vector4::one,true };
	textureParam_[Controller] = { "Controller",{525.0f,290.0f},0.0f,{1020.0f,110.0f},2u,Vector4::one,true };
	textureParam_[BackGround] = { "BackGround",{1280.0f,720.0f},0.0f,{640.0f,360.0f},1u,Color::Convert(0xffffffaa),true };

	for (uint32_t index = 0; index < kMaxTextures_; index++) {
		auto& sprite = texture_.emplace_back(std::make_unique<Sprite>());
		sprite->SetTexture(ResourceManager::GetInstance()->FindTexture(textureParam_[index].name_));
		sprite->SetTexcoordRect({ 0.0f,0.0f }, { sprite->GetTexture()->GetWidth(), sprite->GetTexture()->GetHeight() });
		sprite->SetScale(textureParam_[index].scale_);
		sprite->SetRotate(textureParam_[index].rotate_);
		sprite->SetPosition(textureParam_[index].position_);
		sprite->SetDrawOrder(textureParam_[index].order_);
		sprite->SetColor(textureParam_[index].color_);
		sprite->SetIsActive(textureParam_[index].isActive_);
	}

	isSelected_ = false;
	preIsSelected_ = false;
	easeTime_ = 0.0f;

	changeFlag_ = false;

}

void GamePause::Update() {
#ifdef _DEBUG
	ImGui::Begin("GamePose");
	for (auto& param : textureParam_) {
			ImGui::DragFloat2("pos", &param.position_.x);
		if (ImGui::TreeNode(std::string(param.name_).c_str())) {
			ImGui::DragFloat2("scale", &param.scale_.x);
			ImGui::TreePop();
		}
	}
	ImGui::End();
#endif // _DEBUG

	SelectUpdate();

	TextureUpdate();
}

void GamePause::SetDraw(const bool& flag) {
	for (uint32_t i = 0; i < kMaxTextures_; i++) {
		texture_[i]->SetIsActive(flag);
	}
}

void GamePause::TextureUpdate() {
	for (uint32_t i = 0; i < kMaxTextures_; i++) {
		texture_[i]->SetScale(textureParam_[i].scale_);
		texture_[i]->SetRotate(textureParam_[i].rotate_);
		texture_[i]->SetPosition(textureParam_[i].position_);
		texture_[i]->SetColor(textureParam_[i].color_);
	}
	SetDraw(true);
}

void GamePause::SelectUpdate() {
	auto input = Input::GetInstance();
	auto& xInput = input->GetXInputState();
	auto& preXInput = input->GetPreXInputState();

	if (xInput.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		isSelected_ = false;
	}
	else if (xInput.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		isSelected_ = true;
	}

	if (isSelected_ != preIsSelected_) {
		easeTime_ = 0.0f;
	}
	preIsSelected_ = isSelected_;

	if (changeFlag_) {
		auto trans = Transition::GetInstance();
		if (isSelected_) {
			trans->SetComeToStage();
			if (trans->Update()) {
				// シーンのシングルトンの取得
				SceneManager* sceneManager = SceneManager::GetInstance();
				// シーンの設定
				sceneManager->ChangeScene<TitleScene>();
			}
		}
		else {
			trans->SetComeToStage(trans->getNum());
			if (trans->Update()) {
				// シーンのシングルトンの取得
				SceneManager* sceneManager = SceneManager::GetInstance();
				// シーンの設定
				sceneManager->ChangeScene<BattleScene>();
			}
		}
	}
	else {
		constexpr const float b = 1.0f / 60.0f;
		easeTime_ += b;
		easeTime_ = std::clamp(easeTime_, 0.0f, 1.0f);
		float T = Easing::EaseOutQuart(easeTime_);

		if (isSelected_) {
			textureParam_[Restart].color_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
			textureParam_[StageSelect].color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

			textureParam_[Restart].scale_ = Vector2(900.0f, 256.0f);
			textureParam_[StageSelect].scale_ = Vector2::Slerp(T, Vector2(900.0f, 256.0f), Vector2(1000.0f, 300.0f));
		}
		else {
			textureParam_[Restart].color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			textureParam_[StageSelect].color_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

			textureParam_[Restart].scale_ = Vector2::Slerp(T, Vector2(900.0f, 256.0f), Vector2(1000.0f, 300.0f));
			textureParam_[StageSelect].scale_ = Vector2(900.0f, 256.0f);
		}

		if (xInput.Gamepad.wButtons & XINPUT_GAMEPAD_A && !(preXInput.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
			changeFlag_ = true;
		}
	}
}
