#include "DebugCamera.h"

#include "Graphics/RenderManager.h"
#include "Graphics/ImGuiManager.h"
#include "Input/Input.h"

void DebugCamera::Initialize() {
	// 生成
	camera_ = std::make_shared<Camera>();
	RenderManager::GetInstance()->SetCamera(camera_);

	transform_ = std::make_shared<Transform>();
	//	X軸回転、俯瞰視点
	transform_->rotate = Quaternion::MakeForXAxis(00.0f * Math::ToRadian);
	transform_->translate = Vector3(0.0f, 0.0f, -30.0f);
	eulerAngle_ = {};
}

void DebugCamera::Update() {
	Input* input = Input::GetInstance();

	auto mouseMoveX = input->GetMouseMoveX();
	auto mouseMoveY = input->GetMouseMoveY();
	auto mouseWheel = input->GetMouseWheel();

	if (input->IsMousePressed(1)) {
		constexpr float rotateSpeed = 1.0f * Math::ToRadian;
		eulerAngle_.x += rotateSpeed* static_cast<float>(mouseMoveY) * 0.1f;
		eulerAngle_.y += rotateSpeed* static_cast<float>(mouseMoveX) * 0.1f;
		eulerAngle_.x = std::fmod(eulerAngle_.x, Math::TwoPi);
		eulerAngle_.y = std::fmod(eulerAngle_.y, Math::TwoPi);
		transform_->rotate = Quaternion::MakeFromEulerAngle(eulerAngle_);
	}
	else if (input->IsMousePressed(2)) {
		Vector3 cameraX = transform_->rotate.GetRight() * (-static_cast<float>(mouseMoveX) * 0.01f);
		Vector3 cameraY = transform_->rotate.GetUp() * (static_cast<float>(mouseMoveY) * 0.01f);
		transform_->translate += cameraX + cameraY;
	}
	else if (mouseWheel != 0) {
		Vector3 cameraZ = transform_->rotate.GetForward() * (static_cast<float>(mouseWheel / 120) * 0.5f);
		transform_->translate += cameraZ;
	}

	camera_->SetPosition(transform_->translate);
	camera_->SetRotate(transform_->rotate);
	camera_->UpdateMatrices();
}
