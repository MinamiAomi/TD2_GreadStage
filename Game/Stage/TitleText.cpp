#include "TitleText.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/ImGuiManager.h"

void TitleText::Initialize() {
    SetName("TitleText");
    model_ = std::make_unique<ModelInstance>();

    model_->SetModel(ResourceManager::GetInstance()->FindModel("TitleText"));
    model_->SetIsActive(true);
    model_->SetColor(Vector3::one);

    rotate_ = Vector3(0.0f,0.0f,0.0f);
    transform.rotate = Quaternion::MakeFromEulerAngle(rotate_ * Math::ToRadian);
    transform.translate = Vector3(0.0f, 10.0f, -54.0f);

}

void TitleText::Update() {
    transform.UpdateMatrix();
    model_->SetWorldMatrix(transform.worldMatrix);
}

void TitleText::DrawImGui() {
#ifdef _DEBUG
    ImGui::Begin("TitleText");
    ImGui::DragFloat3("scale", &transform.scale.x, 0.1f);
    ImGui::DragFloat3("rotate", &rotate_.x, 1.0f, -360.0f, 360.0f);
    transform.rotate = Quaternion::MakeFromEulerAngle(rotate_ * Math::ToRadian);
    ImGui::DragFloat3("translate", &transform.translate.x, 0.1f);
    ImGui::End();
#endif // DEBUG
}

void TitleText::AnimationUpdate() {

}
