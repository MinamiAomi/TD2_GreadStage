#include "CameraAnimation.h"

#include <algorithm>

#include "Graphics/RenderManager.h"
#include "Graphics/ImGuiManager.h"
#include "Collision/CollisionManager.h"
#include "CollisionConfig.h"
#include "Input/Input.h"

void CameraAnimation::Initialize() {
    SetName("Camera");
    // 生成
    camera_ = std::make_shared<Camera>();
    RenderManager::GetInstance()->SetCamera(camera_);

    angles_.x = 0.0f;
    angles_.y = Math::Pi;
    
    //	X軸回転、俯瞰視点
    transform.translate = Vector3(0.0f, 10.0f, -12.5f);
    transform.rotate = Quaternion::MakeLookRotation(-transform.translate);
    
    if (nowTitle_) {
        transform.translate = Vector3(0.0f, 10.0f, -50.0f);
        transform.rotate = Quaternion::MakeForYAxis(angles_.y);
        isLeaved_ = true;
        preIsLeaved_ = true;
        isTitleMove_ = false;
    }

    destinationTranslate_ = transform.translate;
    destinationRotate_ = transform.rotate;

    offset_ = Vector3{ 0.0f, 2.0f, 0.0f };
    upDown.x = -70.0f;
    upDown.y = 70.0f;
}

void CameraAnimation::Update() {
    
    nowTitle_ ? TitleUpdate() : NormalUpdate();

    TransUpdate();
}

void CameraAnimation::Restart() {

}

void CameraAnimation::DrawImGui() {
#ifdef _DEBUG
    ImGui::Begin("camera");
    ImGui::DragFloat3("Position", &transform.translate.x, 0.01f);
    ImGui::DragFloat2("radR", &angles_.x, 0.1f);
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

void CameraAnimation::NormalUpdate() {
    if (target_) {
        UpdateInput();

        // 注視点
        Vector3 localTarget = offset_;
        Quaternion localRotate = Quaternion::MakeForYAxis(angles_.y) * Quaternion::MakeForXAxis(angles_.x);
        // 視線
        Vector3 localDir = localRotate.GetForward();

        Vector3 localPosition = localTarget + -localDir * distance_;

        destinationTranslate_ = localPosition * target_->worldMatrix;

        destinationTargetPosition_ = localTarget * target_->worldMatrix;

        destinationRotate_ = target_->rotate * localRotate;
    }
    transform.rotate = Quaternion::Slerp(1.0f - followDelay_, transform.rotate, destinationRotate_);
    lastTargetPosition_ = Vector3::Lerp(1.0f - followDelay_, lastTargetPosition_, destinationTargetPosition_);

    Vector3 origin = lastTargetPosition_;
    Vector3 diff = -transform.rotate.GetForward() * distance_;
    RayCastInfo info{};
    bool rayCastResult = CollisionManager::GetInstance()->RayCast(origin, diff, CollisionConfig::Stage, &info);
    if (!rayCastResult) {
        info.nearest = 1.0f;
    }
    transform.translate = lastTargetPosition_ + diff * info.nearest;

    if (rayCastResult) {
        auto nearestInfo = CollisionManager::GetInstance()->NearestCollider(transform.translate, CollisionConfig::Stage);
        transform.translate += nearestInfo.normal * 0.5f;
    }
}

void CameraAnimation::TitleUpdate() {
    static bool flag = false;
    if (isLeaved_ != preIsLeaved_) {
        flag = true;
    }
    preIsLeaved_ = isLeaved_;
    if (flag) {
        isTitleMove_ = !isTitleMove_;
        if (isTitleMove_) {
            angles_ = preAngles_;
            lastTargetPosition_ = Vector3(0.0f, 10.0f, -50.0f);
        }
        else {
            preAngles_ = angles_;
            angles_.y = Math::Pi;
            angles_.x = 0.0f;
            destinationTranslate_ = Vector3(0.0f, 10.0f, -50.0f);
            destinationRotate_ = Quaternion::MakeForYAxis(angles_.y);
        }
        flag = false;
    }

    DrawImGui();
    if (isTitleMove_) {
        NormalUpdate();
    }
    else {
        transform.rotate = Quaternion::Slerp(1.0f - followDelay_, transform.rotate, destinationRotate_);
        transform.translate = Vector3::Lerp(1.0f - followDelay_, transform.translate, destinationTranslate_);
    }
}

void CameraAnimation::SetCamera() {
    RenderManager::GetInstance()->SetCamera(camera_);
}
