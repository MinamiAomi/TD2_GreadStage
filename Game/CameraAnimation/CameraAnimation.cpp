#include "CameraAnimation.h"

#include "Graphics/RenderManager.h"

void CameraAnimation::Initialize() {
	// 生成
	camera_ = std::make_shared<Camera>();
	RenderManager::GetInstance()->SetCamera(camera_);

	transform_ = std::make_shared<Transform>();
	//	X軸回転、俯瞰視点
	transform_->rotate = Quaternion::MakeForXAxis(50.0f * Math::ToRadian);
	transform_->translate = Vector3(0.0f, 20.0f, -30.0f);
	offset_ = transform_->translate;

}

void CameraAnimation::Update() {
	/*ImGui::DragFloat3("cametrans", &transform_->translate.x, 0.1f);
	static float rot = 45.0f;
	ImGui::DragFloat("rotate", &rot, 1.0f);
	transform_->rotate = Quaternion::MakeForXAxis(rot * Math::ToRadian);*/

	if (target_) {
		transform_->translate.x = target_->worldMatrix.GetTranslate().x;
	}

	camera_->SetPosition(transform_->translate);
	camera_->SetRotate(transform_->rotate);
	camera_->UpdateMatrices();
}

void CameraAnimation::Restart() {

}
