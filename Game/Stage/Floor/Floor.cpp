#include "Floor.h"

#include "Graphics/ResourceManager.h"
#include "Collision/CollisionManager.h"

void Floor::Initialize(const Vector3& basePosition, float radian) {
    SetName("Floor");
    model_ = std::make_unique<ToonModelInstance>();
    collider_ = std::make_unique<BoxCollider>();

    model_->SetModel(ResourceManager::GetInstance()->FindModel("Floor"));
    model_->SetIsActive(true);
    model_->SetUseOutline(false);


    basePosition_ = basePosition;
    transform.translate = basePosition;
    transform.rotate = Quaternion::MakeForZAxis(radian);
    transform.scale = Vector3::one;
    transform.UpdateMatrix();

}

void Floor::Update() {
    
    // 当たり判定、描画を更新
    transform.UpdateMatrix();
    model_->SetWorldMatrix(transform.worldMatrix);
}