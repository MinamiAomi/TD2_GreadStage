#include "CameraAnimation.h"

#include "Graphics/RenderManager.h"
#include "Graphics/ImGuiManager.h"

void CameraAnimation::Initialize() {
	// 生成
	camera_ = std::make_shared<Camera>();
	RenderManager::GetInstance()->SetCamera(camera_);

	transform_ = std::make_shared<Transform>();
	//	X軸回転、俯瞰視点
	transform_->rotate = Quaternion::MakeForXAxis(15.0f * Math::ToRadian);
}

void CameraAnimation::Update() {

	ImGui::Begin("Camera");
	ImGui::DragFloat3("trans", &transform_->translate.x);
	ImGui::End();

	camera_->SetPosition(transform_->translate);
	camera_->SetRotate(transform_->rotate);
	camera_->UpdateMatrices();
}

void CameraAnimation::Restart() {

}
