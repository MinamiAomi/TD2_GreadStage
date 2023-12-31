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

#include "Stage/Stage.h"

void GamePause::Initialize(const uint32_t& num) {
	
	hudPosition_.at(0) = {Vector2(340.0f,100.0f),Vector2(1075.0f,655.0f)};
	hudPosition_.at(1) = {Vector2(100.0f,100.0f),Vector2( 975.0f,655.0f)};
	hudPosition_.at(2) = {Vector2(100.0f,100.0f),Vector2(1075.0f,655.0f)};
	hudPosition_.at(3) = {Vector2(100.0f,100.0f),Vector2(1175.0f,655.0f)};

	pausePosition_.at(0) = {Vector2(570.0f,170.0f),Vector2(647.0f,390.0f)};
	pausePosition_.at(1) = {Vector2(160.0f,160.0f),Vector2(475.0f,390.0f)};
	pausePosition_.at(2) = {Vector2(160.0f,160.0f),Vector2(645.0f,390.0f)};
	pausePosition_.at(3) = {Vector2(160.0f,160.0f),Vector2(820.0f,390.0f)};

	textureParam_[StageSelect] = { "StageSelect",{900.0f,256.0f},0.0f,{425.0f,73.0f},2u,Vector4(0.02f,0.63f,0.01f,1.0f),true };
	textureParam_[Restart] = { "Restart",{900.0f,256.0f},0.0f,{325.0f,175.0f},2u,Vector4(0.02f,0.63f,0.01f,1.0f),true };
	textureParam_[Pose] = { "Pose",{900.0f,256.0f},0.0f,{645.0f,623.0f},2u,Vector4::one,true };
	textureParam_[Controller] = { "Controller",{525.0f,290.0f},0.0f,{1020.0f,110.0f},2u,Vector4::one,true };
	textureParam_[BackGround] = { "BackGround",{1280.0f,720.0f},0.0f,{640.0f,360.0f},1u,Color::Convert(0xffffffaa),true };
	//textureParam_[Frame] = { "Frame",{570.0f,170.0f},0.0f,{647.0f,390.0f},1u,Vector4::one,true };
	textureParam_[Frame] = { "Frame",pausePosition_.at(0).scale_,0.0f,pausePosition_.at(0).translate_,1u,Vector4::one,true};
	//textureParam_[Moon1] = { "Moon",{160.0f,160.0f},0.0f,{502.0f,390.0f},1u,Vector4::one,false };
	textureParam_[Moon1] = { "Moon",pausePosition_.at(1).scale_,0.0f,pausePosition_.at(1).translate_,1u,Vector4::one,false };
	//textureParam_[Moon2] = { "Moon",{160.0f,160.0f},0.0f,{675.0f,390.0f},1u,Vector4::one,false };
	textureParam_[Moon2] = { "Moon",pausePosition_.at(2).scale_,0.0f,pausePosition_.at(2).translate_,1u,Vector4::one,false };
	//textureParam_[Moon3] = { "Moon",{160.0f,160.0f},0.0f,{848.0f,390.0f},1u,Vector4::one,false };
	textureParam_[Moon3] = { "Moon",pausePosition_.at(3).scale_,0.0f,pausePosition_.at(3).translate_,1u,Vector4::one,false };

	for (uint32_t index = 0; index < TextureName::MaxTexture; index++) {
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

	optionParam_ = { "Option",{128.0f,128.0f},0.0f,{100.0f,640.0f},1u,Vector4::one,true };
	optionText_ = std::make_unique<Sprite>();
	optionText_->SetTexture(ResourceManager::GetInstance()->FindTexture(optionParam_.name_));
	optionText_->SetTexcoordRect({ 0.0f,0.0f }, { optionText_->GetTexture()->GetWidth(), optionText_->GetTexture()->GetHeight() });
	optionText_->SetScale(optionParam_.scale_);
	optionText_->SetRotate(optionParam_.rotate_);
	optionText_->SetPosition(optionParam_.position_);
	optionText_->SetDrawOrder(optionParam_.order_);
	optionText_->SetColor(optionParam_.color_);
	optionText_->SetIsActive(optionParam_.isActive_);

	isSelected_ = false;
	preIsSelected_ = false;
	easeTime_ = 0.0f;

	changeFlag_ = false;

	noSelectColor_ = Vector4(0.42f, 0.44f, 0.45f, 1.0f);
	sceneNumber = num;
}

void GamePause::Update() {
#ifdef _DEBUG
	ImGui::Begin("GamePose");
	int i = 0;
	for (auto& param : textureParam_) {
		if (ImGui::TreeNode(std::string(param.name_ + std::to_string(i)).c_str())) {
			ImGui::DragFloat2("pos", &param.position_.x);
			ImGui::DragFloat2("scale", &param.scale_.x);
			ImGui::TreePop();
		}i++;
	}
	ImGui::DragFloat2("testPos", &optionParam_.position_.x);
	ImGui::DragFloat2("testscale", &optionParam_.scale_.x);
	optionText_->SetScale(optionParam_.scale_);
	optionText_->SetPosition(optionParam_.position_);
	ImGui::End();
#endif // _DEBUG

	SetDraw(true);

	SelectUpdate();

	for (uint32_t index = 0; index < 4u; index++) {
		textureParam_[Frame + index].isActive_ = true;
		textureParam_[Frame + index].scale_ = pausePosition_.at(index).scale_;
		textureParam_[Frame + index].position_ = pausePosition_.at(index).translate_;
	}
	for (uint32_t index = 0; index < Stage::ItemCount_; index++) {
		textureParam_[Moon3 - index].isActive_ = false;
	}

	TextureUpdate();
}

void GamePause::SetDraw(const bool& flag) {
	for (uint32_t i = 0; i < TextureName::MaxTexture; i++) {
		textureParam_[i].isActive_ = flag;
		texture_[i]->SetIsActive(flag);
	}
}

void GamePause::MoonHudUpdate() {
	for (uint32_t index = 0; index < 4u; index++) {
		textureParam_[Frame + index].isActive_ = true;
		textureParam_[Frame + index].scale_ = hudPosition_.at(index).scale_;
		textureParam_[Frame + index].position_ = hudPosition_.at(index).translate_;
	}
	for (uint32_t index = 0; index < Stage::ItemCount_; index++) {
		textureParam_[Moon3 - index].isActive_ = false;
	}
	TextureUpdate();
}

void GamePause::TextureUpdate() {
	for (uint32_t i = 0; i < TextureName::MaxTexture; i++) {
		texture_[i]->SetScale(textureParam_[i].scale_);
		texture_[i]->SetRotate(textureParam_[i].rotate_);
		texture_[i]->SetPosition(textureParam_[i].position_);
		texture_[i]->SetColor(textureParam_[i].color_);
		texture_[i]->SetIsActive(textureParam_[i].isActive_);
	}
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
			if (sceneNumber == 0) {
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
	}
	else {
		constexpr const float b = 1.0f / 60.0f;
		easeTime_ += b;
		easeTime_ = std::clamp(easeTime_, 0.0f, 1.0f);
		float T = Easing::EaseOutQuart(easeTime_);

		if (isSelected_) {
			textureParam_[Restart].color_ = noSelectColor_;
			textureParam_[StageSelect].color_ = Vector4(0.02f, 0.63f, 0.01f, 1.0f);

			textureParam_[Restart].scale_ = Vector2(900.0f, 256.0f);
			textureParam_[StageSelect].scale_ = Vector2::Slerp(T, Vector2(900.0f, 256.0f), Vector2(1000.0f, 300.0f));
		}
		else {
			textureParam_[Restart].color_ = Vector4(0.02f, 0.63f, 0.01f, 1.0f);
			textureParam_[StageSelect].color_ = noSelectColor_;

			textureParam_[Restart].scale_ = Vector2::Slerp(T, Vector2(900.0f, 256.0f), Vector2(1000.0f, 300.0f));
			textureParam_[StageSelect].scale_ = Vector2(900.0f, 256.0f);
		}

		if (xInput.Gamepad.wButtons & XINPUT_GAMEPAD_A && !(preXInput.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
			changeFlag_ = true;
		}
	}
}
