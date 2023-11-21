#include "CameraAnimation.h"

#include "Graphics/RenderManager.h"
#include "Graphics/ImGuiManager.h"

void CameraAnimation::Initialize() {
	SetName("Camera");
	// 生成
	camera_ = std::make_shared<Camera>();
	RenderManager::GetInstance()->SetCamera(camera_);

	//	X軸回転、俯瞰視点
	transform.translate = Vector3(0.0f, 10.0f, -10.0f);
	transform.rotate = Quaternion::MakeLookRotation(-transform.translate);
	offset_ = transform.translate + Vector3{0.0f, -8.0f, 0.0f};

}

void CameraAnimation::Update() {
	
	if (target_) {
		//transform_->translate.x = target_->worldMatrix.GetTranslate().x;
		transform.rotate = target_->rotate;
		transform.translate = target_->translate + (transform.rotate * offset_);
	}
	//ImGui::Begin("camera");
	//ImGui::DragFloat3("Position", &transform_->translate.x, 0.01f);	
	//static Vector3 r = {0.0f, 0.0f,0.0f};
	//ImGui::DragFloat3("Rotate", &r.x, 0.1f, -360.0f, 360.0f);
	//transform_->rotate = Quaternion::MakeFromEulerAngle(r * Math::ToRadian);
	//ImGui::End();
	transform.UpdateMatrix();
	camera_->SetPosition(transform.translate);
	camera_->SetRotate(transform.rotate);
	camera_->UpdateMatrices();

}

void CameraAnimation::Restart() {

}
