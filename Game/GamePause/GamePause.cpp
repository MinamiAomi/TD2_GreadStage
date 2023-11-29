#include "GamePause.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/ImGuiManager.h"

void GamePause::Initialize() {
	stageSelect_ = std::make_unique<Sprite>();
	stageSelect_->SetTexture(ResourceManager::GetInstance()->FindTexture("StageSelect"));
	stageSelectParam_.scale_ = Vector2(900.0f,256.0f);
	stageSelectParam_ = { {900.0f,256.0f},0.0f,{0.0f,0.0f},1u,Vector4::one,true };
	TextureInitialize(stageSelect_.get(), stageSelectParam_);

	restart_ = std::make_unique<Sprite>();
	restart_->SetTexture(ResourceManager::GetInstance()->FindTexture("Restart"));
	restartParam_ = { {900.0f,256.0f},0.0f,{0.0f,0.0f},2u,Vector4::one,true };
	TextureInitialize(restart_.get(), restartParam_);

	pose_ = std::make_unique<Sprite>();
	pose_->SetTexture(ResourceManager::GetInstance()->FindTexture("Pose"));
	poseParam_ = { {900.0f,256.0f},0.0f,{0.0f,0.0f},3u,Vector4::one,true };
	TextureInitialize(pose_.get(), poseParam_);

	controller_ = std::make_unique<Sprite>();
	controller_->SetTexture(ResourceManager::GetInstance()->FindTexture("Controller"));
	controllerParam_ = { {1372.0f,738.0f},0.0f,{0.0f,0.0f},4u,Vector4::one,true };
	TextureInitialize(controller_.get(), controllerParam_);

	backGround_ = std::make_unique<Sprite>();
	backGround_->SetTexture(ResourceManager::GetInstance()->FindTexture("BackGround"));
	backGroundParam_ = { {1280.0f,720.0f},0.0f,{0.0f,0.0f},5u,Vector4::one,true };
	TextureInitialize(backGround_.get(), backGroundParam_);

}

void GamePause::Update() {
	ImGui::Begin("GamePose");
	if (ImGui::TreeNode("stageSelect")) {
		ImGui::DragFloat2("pos",&stageSelectParam_.position_.x);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("restart")) {
		ImGui::DragFloat2("pos", &restartParam_.position_.x);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("poseParam_")) {
		ImGui::DragFloat2("pos", &poseParam_.position_.x);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("controllerParam_")) {
		ImGui::DragFloat2("pos", &controllerParam_.position_.x);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("backGround_")) {
		ImGui::DragFloat2("pos", &backGroundParam_.position_.x);
		ImGui::TreePop();
	}
	ImGui::End();

	TextureInitialize(stageSelect_.get(), stageSelectParam_);
	TextureInitialize(restart_.get(), restartParam_);
	TextureInitialize(pose_.get(), poseParam_);
	TextureInitialize(controller_.get(), controllerParam_);
	TextureInitialize(backGround_.get(), backGroundParam_);

}

void GamePause::TextureInitialize(Sprite* sprite, TextureParam param) {
	sprite->SetScale(param.scale_);
	sprite->SetRotate(param.rotate_);
	sprite->SetPosition(param.position_);
	sprite->SetTexcoordRect({0.0f,0.0f}, { sprite->GetTexture()->GetWidth(), sprite->GetTexture()->GetHeight() });
	//sprite->SetDrawOrder(param.order_);
	//sprite->SetColor(param.color_);
	//sprite->SetIsActive(param.isActive_);
}
