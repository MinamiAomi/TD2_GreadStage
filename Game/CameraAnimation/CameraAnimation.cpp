#include "CameraAnimation.h"

#include "Graphics/RenderManager.h"
#include "Graphics/ImGuiManager.h"
#include "Input/Input.h"
#include <algorithm>

void CameraAnimation::Initialize() {
	SetName("Camera");
	// 生成
	camera_ = std::make_shared<Camera>();
	RenderManager::GetInstance()->SetCamera(camera_);

	//	X軸回転、俯瞰視点
	transform.translate = Vector3(0.0f, 10.0f, -12.5f);
	transform.rotate = Quaternion::MakeLookRotation(-transform.translate);
	offset_ = transform.translate + Vector3{0.0f, -8.0f, 0.0f};
	upDown.x = -10.0f;
	upDown.y = 20.0f;
}

void CameraAnimation::Update() {
	

	DrawImGui();

	if (target_) {
		//transform_->translate.x = target_->worldMatrix.GetTranslate().x;
		auto input = Input::GetInstance();
		auto& xinput = input->GetXInputState();
		if (input->IsKeyPressed(DIK_LEFTARROW)
			|| xinput.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
			radRot_.y -= 1.0f;
		}
		if (input->IsKeyPressed(DIK_RIGHTARROW)
			|| xinput.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
			radRot_.y += 1.0f;
		}
		if (input->IsKeyPressed(DIK_UPARROW)
			|| xinput.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
			radRot_.x -= 1.0f;
		}
		if (input->IsKeyPressed(DIK_DOWNARROW)
			|| xinput.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
			radRot_.x += 1.0f;
		}
		
		
		// 制限
		if (std::abs(radRot_.y) >= 360.0f) {
			radRot_.y = 0.0f;
		}
		radRot_.x = std::clamp(radRot_.x, upDown.x, upDown.y);

		// Y軸回転
		transform.rotate = Quaternion::MakeFromAngleAxis(radRot_.y * Math::ToRadian, target_->rotate.GetUp());
		// X軸回転
		Quaternion r = Quaternion::MakeFromAngleAxis(radRot_.x * Math::ToRadian, target_->rotate.GetRight());
		transform.rotate *= r;
		transform.rotate *= target_->rotate;
		transform.translate = target_->translate + (transform.rotate * offset_);
	}
	
	TransUpdate();
}

void CameraAnimation::Restart() {

}

void CameraAnimation::DrawImGui() {
#ifdef _DEBUG
	ImGui::Begin("camera");
	ImGui::DragFloat3("Position", &offset_.x, 0.01f);
	ImGui::DragFloat3("radR", &radRot_.x, 0.01f);
	ImGui::DragFloat2("Limit Down:Up", &upDown.x, 0.01f);
	ImGui::End();
#endif // _DEBUG
}

void CameraAnimation::TransUpdate() {
	transform.UpdateMatrix();
	camera_->SetPosition(transform.translate);
	camera_->SetRotate(transform.rotate);
	camera_->UpdateMatrices();
}

void CameraAnimation::SetCamera() {
	RenderManager::GetInstance()->SetCamera(camera_);
}
