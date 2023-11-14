#include "CameraAnimation.h"

#include "Graphics/RenderManager.h"
#include "Graphics/ImGuiManager.h"

void CameraAnimation::Initialize() {
	// 生成
	camera_ = std::make_shared<Camera>();
	RenderManager::GetInstance()->SetCamera(camera_);

	transform_ = std::make_shared<Transform>();
	//	X軸回転、俯瞰視点
	transform_->rotate = Quaternion::MakeForXAxis(20.0f * Math::ToRadian);
	transform_->translate = Vector3(0.0f, 2.0f, -10.0f);
	offset_ = transform_->translate;

}

void CameraAnimation::Update() {
	
	if (target_) {
		//transform_->translate.x = target_->worldMatrix.GetTranslate().x;
		transform_->rotate = target_->rotate;
		transform_->translate = target_->translate + (transform_->rotate * offset_);
	}
	//ImGui::Begin("camera");
	//ImGui::DragFloat3("Position", &transform_->translate.x, 0.01f);	
	//static Vector3 r = {0.0f, 0.0f,0.0f};
	//ImGui::DragFloat3("Rotate", &r.x, 0.1f, -360.0f, 360.0f);
	//transform_->rotate = Quaternion::MakeFromEulerAngle(r * Math::ToRadian);
	//ImGui::End();

	camera_->SetPosition(transform_->translate);
	camera_->SetRotate(transform_->rotate);
	camera_->UpdateMatrices();

}

void CameraAnimation::Restart() {

}
