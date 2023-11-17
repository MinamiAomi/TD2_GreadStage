#include "Player.h"

#include "Graphics/ResourceManager.h"
#include "Engine/Input/Input.h"
#include "Collision/CollisionManager.h"

#include "Graphics/ImGuiManager.h"
#include <numbers>
#include "CollisionConfig.h"

void Player::Initialize() {
    SetName("Player");

    // 初期座標の設定
    transform.translate = Vector3(0.0f, 2.0f, 0.0f);
    //transform.rotate = Quaternion::MakeForYAxis(0.45f);

    // モデルの取得
    model_ = std::make_unique<ModelInstance>();
    model_->SetModel(ResourceManager::GetInstance()->FindModel("Player"));
    model_->SetIsActive(true);

    // 座標更新してからでなければローカルデータが消えてしまう
    modelTrans_.SetParent(&transform);
    modelTrans_.translate = {};
    modelTrans_.UpdateMatrix();

    collider_ = std::make_unique<SphereCollider>();
    collider_->SetGameObject(this);
    collider_->SetName("Player");
    collider_->SetCenter(transform.translate + colliderOffset_);
    collider_->SetRadius(0.5f);
    collider_->SetCollisionAttribute(CollisionConfig::Player);
    collider_->SetCollisionMask(~CollisionConfig::Player);
    //collider_->SetSize({ 1.0f, 2.0f, 1.0f });
    collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });

    // パラメーター初期化
    moveSpeed_ = 0.1f;
    jumpParamerets_.isJumped_ = false;
    jumpParamerets_.fallSpeed_ = 0.0f;
    jumpParamerets_.jumpPower_ = 1.0f;

    floorCollider_ = nullptr;
}

void Player::Update() {
#ifdef _DEBUG
    DrawImGui();
    if (Input::GetInstance()->IsKeyTrigger(DIK_R)) {
        transform.translate = Vector3::zero;
    }
#endif // DEBUG

    MoveUpdate();
    JumpUpdate();

    MoveLimit();

    UpdateTransform();
}

void Player::PreCollisionUpdate() {
    if (moveDirection_.Length() <= 0.00001f) {
        return;
    }

    wallColliders_.clear();
    auto collisionManager = CollisionManager::GetInstance();

    float angles[] = {
        0.0f,
        45.0f * Math::ToRadian,
       -45.0f * Math::ToRadian,
        90.0f * Math::ToRadian,
       -90.0f * Math::ToRadian,
    };
    Vector3 axis = transform.rotate.GetUp();
    for (uint32_t i = 0; i < _countof(angles); ++i) {
        RayCastInfo rayCastInfo{};
        Vector3 origin = colliderOffset_ * transform.worldMatrix;
        Vector3 direction = Quaternion::MakeFromAngleAxis(angles[i], axis) * moveDirection_.Normalized() * 0.6f;
        if (collisionManager->RayCast(origin, direction, CollisionConfig::Stage, &rayCastInfo)) {
            wallColliders_.emplace_back(rayCastInfo.collider);
        }
    }
}

void Player::UpdateTransform() {
    // 座標更新
    transform.UpdateMatrix();
    modelTrans_.UpdateMatrix();

    collider_->SetCenter(colliderOffset_ * transform.worldMatrix);
    overlapCollider_->SetCenter(colliderOffset_ * transform.worldMatrix);
    //collider_->SetOrientation(rotate);

    // モデル座標更新
    model_->SetWorldMatrix(modelTrans_.worldMatrix);
}

void Player::MoveUpdate() {

    auto input = Input::GetInstance();
    Vector3 move;

    // キーボードでの移動
    if (input->IsKeyPressed(DIK_W)) {
        move.z += moveSpeed_;
    }
    if (input->IsKeyPressed(DIK_S)) {
        move.z -= moveSpeed_;
    }
    if (input->IsKeyPressed(DIK_A)) {
        move.x -= moveSpeed_;
    }
    if (input->IsKeyPressed(DIK_D)) {
        move.x += moveSpeed_;
    }

    auto xinput = input->GetXInputState();
    if (std::abs(xinput.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        move.x = 0.0f;
        move.x = xinput.Gamepad.sThumbLX;
    }
    if (std::abs(xinput.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        move.z = 0.0f;
        move.z = xinput.Gamepad.sThumbLY;
    }

    // 移動処理
    if (move != Vector3::zero) {
        move = move.Normalized();

        move = camera_->GetCamera()->GetRotate() * move;
        move *= moveSpeed_;
        transform.translate += move;
    }

    moveDirection_ = move;
}

void Player::MoveLimit() {
    jumpParamerets_.isJumped_ = true;
    if (transform.translate.y <= -10.0f) {
        jumpParamerets_.isJumped_ = false;
        transform.translate = Vector3(0.0f, 2.0f, 0.0f);
        transform.rotate = Quaternion::identity;
    }
}

void Player::JumpUpdate() {
    auto input = Input::GetInstance();
    const float gravity = 0.08f;

    if (!jumpParamerets_.isJumped_ && input->IsKeyTrigger(DIK_SPACE)) {
        jumpParamerets_.isJumped_ = true;
        jumpParamerets_.fallSpeed_ = jumpParamerets_.jumpPower_;
    }

    if (jumpParamerets_.isJumped_) {
        jumpParamerets_.fallSpeed_ -= gravity;
        Vector3 gravityDirection = transform.rotate.GetUp();
        transform.translate += gravityDirection * jumpParamerets_.fallSpeed_;
    }
    else {
        jumpParamerets_.fallSpeed_ = 0.0f;
    }
}

void Player::WallUpdate(Vector3 moveVec) {
    moveVec;
}

void Player::OnCollision(const CollisionInfo& collisionInfo) {
    if (collisionInfo.collider->GetName() == "Box") {
        // ワールド空間の押し出しベクトル
        Vector3 pushVector = collisionInfo.normal * collisionInfo.depth;
        transform.translate += pushVector;

        // 衝突位置の法線
        // float dot = Dot(collisionInfo.normal, Vector3::up);
        // //// 地面と見なす角度
        // 
        // const float kGroundGradientAngle = 45.0f * Math::ToRadian;
        // if (std::abs(std::acos(dot)) < kGroundGradientAngle) {
        //     floorCollider_ = collisionInfo.collider;
        // }


        //if (floorCollider_ == collisionInfo.collider) {
            // Quaternionは後ろからかける

        // 事前に飛ばしたレイに当たったコライダーは処理を通さない
        auto wallCollider = std::find(wallColliders_.begin(), wallColliders_.end(), collisionInfo.collider);
        if (wallCollider == wallColliders_.end()) {

            jumpParamerets_.isJumped_ = false;
            jumpParamerets_.fallSpeed_ = 0.0f;

            Vector3 up = transform.rotate.GetUp();
            Vector3 normal = collisionInfo.normal.Normalized();
            if (Dot(up, normal) < 0.9999f) {
                Quaternion diff = Quaternion::MakeFromTwoVector(up, normal);
                transform.rotate = diff * transform.rotate;
            }
        }
        //}
    }


    UpdateTransform();

}

void Player::DrawImGui() {
    ImGui::Begin("test", nullptr, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Player")) {
            if (ImGui::TreeNode("Translate")) {
                ImGui::DragFloat3("trans", &transform.translate.x, 0.1f);
                ImGui::TreePop();
            }
            ImGui::Text("A");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Player2")) {
            if (ImGui::TreeNode("Rotation")) {
                ImGui::DragFloat3("rota", &transform.rotate.x, 0.1f);
                ImGui::TreePop();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();
}
