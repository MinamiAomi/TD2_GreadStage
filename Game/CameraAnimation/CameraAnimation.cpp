#include "CameraAnimation.h"

#include <algorithm>

#include "Graphics/RenderManager.h"
#include "Graphics/ImGuiManager.h"
#include "Collision/CollisionManager.h"
#include "CollisionConfig.h"
#include "Input/Input.h"
#include "Easing/Easing.h"


decltype(CameraAnimation::nowTitle_) CameraAnimation::nowTitle_ = true;

void CameraAnimation::Initialize() {
    SetName("Camera");
    // 生成
    camera_ = std::make_shared<Camera>();
    RenderManager::GetInstance()->SetCamera(camera_);

    //	X軸回転、俯瞰視点
    transform.translate = Vector3(0.0f, 10.0f, -12.5f);
    transform.rotate = Quaternion::MakeLookRotation(-transform.translate);
    
    if (nowTitle_) {
        angles_.x = 0.0f;
        angles_.y = Math::Pi;
        transform.translate = Vector3(0.0f, 10.0f, -50.0f);
        transform.rotate = Quaternion::MakeForYAxis(angles_.y);
        isLeaved_ = true;
        preIsLeaved_ = true;
        isTitleMove_ = false;
        nowTitle_ = false;
    }
    destinationTranslate_ = transform.translate;
    destinationRotate_ = transform.rotate;

    offset_ = Vector3{ 0.0f, 2.0f, 0.0f };

    isStageMove_ = true;
}

void CameraAnimation::Update() {
    DrawImGui();

    if (!isDuringReset_) { easeChangeFlag_ = true; }
    isStageMove_ ? StageMoveUpdate() : isDuringReset_ ? ResetMove() : NormalUpdate();
    
    TransUpdate();
}

void CameraAnimation::Restart() {

}

void CameraAnimation::DrawImGui() {
#ifdef _DEBUG
    ImGui::Begin("camera");
    ImGui::DragFloat3("Position", &transform.translate.x, 0.1f);
    ImGui::DragFloat2("rotate", &angles_.x, 0.1f, -360.0f, 360.0f);
    //transform.rotate = Quaternion::MakeFromEulerAngle(Vector3(angles_.x, angles_.y, 0.0f) * Math::ToRadian);
    ImGui::DragFloat("Delay", &followDelay_, 0.01f);
    ImGui::DragFloat("Distance", &distance_, 0.01f);
    ImGui::DragFloat2("Speed", &cameraSpeed_.x, 0.01f);
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

    // ジャンプフラグがあった時に下を向く
    //if(gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_A) { viewMove.x += 1.0f; }

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

    if (isTitleMove_) {
        NormalUpdate();
    }
    else {
        transform.rotate = Quaternion::Slerp(1.0f - followDelay_, transform.rotate, destinationRotate_);
        transform.translate = Vector3::Lerp(1.0f - followDelay_, transform.translate, destinationTranslate_);
    }

    TransUpdate();
}

void CameraAnimation::SetCamera() {
    RenderManager::GetInstance()->SetCamera(camera_);
}

bool CameraAnimation::EaseUpdate() {
    easeCount_ = std::clamp(easeCount_, 0.0f, 1.0f);
    //float T = Easing::EaseOutExpo(easeCount_);
    float T = easeCount_;
    transform.translate = Vector3::Lerp(T, easeStart_.pos, easeEnd_.pos);
    angles_ = Vector2::Lerp(T, easeStart_.angle, easeEnd_.angle);
    transform.rotate = Quaternion::MakeFromEulerAngle(Vector3(angles_.x, angles_.y, 0.0f) * Math::ToRadian);
    if (easeCount_ >= 1.0f) {
        return true;
    }
    easeCount_ += easeSpeed_;
    return false;
}

void CameraAnimation::StageMoveUpdate() {
    
    auto& ease = easeParameter_;
    // 初期化段階
    if (behaviorRequest_) {
        nowFase_ = behaviorRequest_.value();
        easeCount_ = 0.0f;
        switch (nowFase_) {
        case CameraAnimation::Fase::First:
            easeStart_.pos = ease.first.pos;
            easeStart_.angle = ease.first.angle;
            easeEnd_.pos = ease.second.pos;
            easeEnd_.angle = ease.second.angle;
            easeSpeed_ = ease.first.speed;
            break;
        case CameraAnimation::Fase::Second:
            easeStart_.pos = ease.second.pos;
            easeStart_.angle = ease.second.angle;
            easeEnd_.pos = ease.third.pos;
            easeEnd_.angle = ease.third.angle;
            easeSpeed_ = ease.second.speed;
            break;
        case CameraAnimation::Fase::Third:
            easeStart_.pos = ease.third.pos;
            easeStart_.angle = ease.third.angle;
            easeEnd_.pos = ease.fourth.pos;
            easeEnd_.angle = ease.fourth.angle;
            easeSpeed_ = ease.third.speed;
            break;
        case CameraAnimation::Fase::Fourth:
            easeStart_.pos = ease.fourth.pos;
            easeStart_.angle = ease.fourth.angle;
            easeEnd_.pos = ease.fifth.pos;
            easeEnd_.angle = ease.fifth.angle;
            easeSpeed_ = ease.fourth.speed;
            break;
        case CameraAnimation::Fase::Fifth:
            break;
        }
        behaviorRequest_ = std::nullopt;
    }

    // 更新
    switch (nowFase_) {
    case CameraAnimation::Fase::First:
    default:
        if (EaseUpdate()) {
            behaviorRequest_ = CameraAnimation::Fase::Second;
        }
        break;
    case CameraAnimation::Fase::Second:
        if (EaseUpdate()) {
            behaviorRequest_ = CameraAnimation::Fase::Third;
        }
        break;
    case CameraAnimation::Fase::Third:
        if (EaseUpdate()) {
            behaviorRequest_ = CameraAnimation::Fase::Fourth;
        }
        break;
    case CameraAnimation::Fase::Fourth:
        if (EaseUpdate()) {
            behaviorRequest_ = CameraAnimation::Fase::Fifth;
        }
        break;
    case CameraAnimation::Fase::Fifth:
        angles_ = Vector2::zero;
        isStageMove_ = false;
        lastTargetPosition_ = transform.translate;
        break;
    }

}

void CameraAnimation::ResetMove() {
    // 初期設定
    if (easeChangeFlag_) {
        easeStart_.pos = transform.translate;
        easeStart_.angle = angles_;
        if (target_) {
            // 注視点
            Vector3 localTarget = offset_;
            Vector3 diff = -transform.rotate.GetForward() * distance_;
            easeEnd_.pos = localTarget * target_->worldMatrix + diff;
            destinationTargetPosition_ = easeEnd_.pos - diff;
            lastTargetPosition_ = easeEnd_.pos - diff;

            easeEnd_.angle = Vector2(0.0f, 0.0f);
            auto localRotate = Quaternion::MakeFromEulerAngle(Vector3(angles_.x, angles_.y, 0.0f) * Math::ToRadian);
            //destinationRotate_ = target_->rotate * localRotate;
        }
        easeCount_ = 0u;
        easeChangeFlag_ = false;
        easeSpeed_ = 1.0f / 30.0f;
    }

    easeCount_ += easeSpeed_;
    easeCount_ = std::clamp(easeCount_, 0.0f, 1.0f);
    float T = Easing::EaseOutSine(easeCount_);
    transform.translate = Vector3::Lerp(T, easeStart_.pos, easeEnd_.pos);
    //angles_ = Vector2::Lerp(T, easeStart_.angle, easeEnd_.angle);
    //transform.rotate = Quaternion::MakeFromEulerAngle(Vector3(angles_.x, angles_.y, 0.0f) * Math::ToRadian);
}



