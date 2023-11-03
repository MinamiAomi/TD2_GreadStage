#include "CameraAnimation.h"

#include "Graphics/RenderManager.h"

void CameraAnimation::Initialize() {
	// 生成
	camera_ = std::make_shared<Camera>();
	RenderManager::GetInstance()->SetCamera(camera_);

	transform_ = std::make_shared<Transform>();
	//	X軸回転、俯瞰視点
	transform_->rotate = Quaternion::MakeForXAxis(15.0f * Math::ToRadian);
}

void CameraAnimation::Update() {

	if (target_) {
		Vector3 offset = { 0.0f, 3.0f, -10.0f };
		offset = transform_->rotate * offset;
		transform_->translate = target_->worldMatrix.GetTranslate() + offset;
	}

	camera_->SetPosition(transform_->translate);
	camera_->SetRotate(transform_->rotate);
	camera_->UpdateMatrices();
}

void CameraAnimation::Restart() {

}
