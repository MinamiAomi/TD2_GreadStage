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
    offset_ = Vector3{ 0.0f, 2.0f, 0.0f };
    upDown.x = -70.0f;
    upDown.y = 70.0f;
}

void CameraAnimation::Update() {


    DrawImGui();

    UpdateInput();

    if (target_) {
        // 注視点
        Vector3 localTarget = offset_;
        Quaternion localRotate = Quaternion::MakeForYAxis(angles_.y) * Quaternion::MakeForXAxis(angles_.x);
        // 視線
        Vector3 localDir = localRotate.GetForward();

        Vector3 localPosition = localTarget + -localDir * distance_;

        destinationTranslate_ = localPosition * target_->worldMatrix;
        destinationRotate_ = target_->rotate * localRotate;
    }

    //if (target_) {
    //    //transform_->translate.x = target_->worldMatrix.GetTranslate().x;
    //    auto input = Input::GetInstance();
    //    auto& xinput = input->GetXInputState();
    //    const float kSpeed = 2.5f;

    //    if (input->IsKeyPressed(DIK_LEFTARROW)
    //        || xinput.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
    //        radRot_.y -= kSpeed;
    //    }
    //    if (input->IsKeyPressed(DIK_RIGHTARROW)
    //        || xinput.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
    //        radRot_.y += kSpeed;
    //    }
    //    if (input->IsKeyPressed(DIK_UPARROW)
    //        || xinput.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
    //        radRot_.x -= kSpeed;
    //    }
    //    if (input->IsKeyPressed(DIK_DOWNARROW)
    //        || xinput.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
    //        radRot_.x += kSpeed;
    //    }


    //    // 制限
    //    if (std::abs(radRot_.y) >= 360.0f) {
    //        radRot_.y = 0.0f;
    //    }
    //    radRot_.x = std::clamp(radRot_.x, upDown.x, upDown.y);

    //    // Y軸回転
    //    transform.rotate = Quaternion::MakeFromAngleAxis(radRot_.y * Math::ToRadian, target_->rotate.GetUp());
    //    // X軸回転
    //    Quaternion r = Quaternion::MakeFromAngleAxis(radRot_.x * Math::ToRadian, target_->rotate.GetRight());
    //    transform.rotate *= r;
    //    transform.rotate *= target_->rotate;
    //    transform.translate = target_->translate + (transform.rotate * offset_);

    //}

    transform.rotate = Quaternion::Slerp(1.0f - followDelay_, transform.rotate, destinationRotate_);
    transform.translate = Vector3::Lerp(1.0f - followDelay_, transform.translate, destinationTranslate_);

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

void CameraAnimation::UpdateInput() {
    auto input = Input::GetInstance();
    auto& gamepad = input->GetXInputState();

    Vector2 viewMove = { 0.0f,0.0f };
    if (input->IsKeyPressed(DIK_LEFTARROW)) { viewMove.x += -1.0f; }
    if (input->IsKeyPressed(DIK_RIGHTARROW)) { viewMove.x += 1.0f; }
    if (input->IsKeyPressed(DIK_UPARROW)) { viewMove.y += 1.0f; }
    if (input->IsKeyPressed(DIK_DOWNARROW)) { viewMove.y += -1.0f; }

    if (std::abs(gamepad.Gamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
        viewMove.x = 0.0f;
        viewMove.x = -float(gamepad.Gamepad.sThumbRY);
    }
    if (std::abs(gamepad.Gamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
        viewMove.y = 0.0f;
        viewMove.y = float(gamepad.Gamepad.sThumbRX);
    }

    if (viewMove != Vector2::zero) {
        viewMove = Vector2::Scale(viewMove.Normalized(), cameraSpeed_ * Math::ToRadian);
        angles_ += viewMove;
        angles_.x = std::clamp(angles_.x, pitchAngleLimits_.y * Math::ToRadian, pitchAngleLimits_.x * Math::ToRadian);
        angles_.y = std::fmod(angles_.y, Math::TwoPi);
    }
}

void CameraAnimation::SetCamera() {
    RenderManager::GetInstance()->SetCamera(camera_);
}
