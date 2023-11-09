#include "Wall.h"

#include "Graphics/ResourceManager.h"
#include "Collision/CollisionManager.h"

void Wall::Initialize(const Vector3& basePosition, float radian) {
    SetName("Wall");
    model_ = std::make_unique<ToonModelInstance>();
    collider_ = std::make_unique<BoxCollider>();
    colliderWallRange_ = std::make_unique<BoxCollider>();

    model_->SetModel(ResourceManager::GetInstance()->FindModel("Wall"));
    model_->SetIsActive(true);
    model_->SetUseOutline(false);

    basePosition_ = basePosition;
    transform.translate = basePosition;
    transform.rotate = Quaternion::MakeForYAxis(radian);
    transform.scale = Vector3::one;
    transform.UpdateMatrix();

    collider_->SetName("Wall");
    collider_->SetGameObject(this);
    collider_->SetOrientation(transform.rotate);
    collider_->SetSize({ 2.0f, 5.0f, 5.0f });
    collider_->SetCenter(transform.translate);

    colliderWallRange_->SetName("WallRange");
    colliderWallRange_->SetGameObject(this);
    colliderWallRange_->SetOrientation(transform.rotate);
    colliderWallRange_->SetSize({ 2.2f, 5.2f, 5.2f });
    colliderWallRange_->SetCenter(transform.translate);


}

void Wall::Update() {
    
    // 当たり判定、描画を更新
    transform.UpdateMatrix();
    collider_->SetCenter(transform.translate);
    colliderWallRange_->SetCenter(transform.translate);
    model_->SetWorldMatrix(transform.worldMatrix);
}