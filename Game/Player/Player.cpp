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
    transform.translate = Vector3(0.0f, 3.0f, 0.0f);
    //transform.rotate = Quaternion::MakeForYAxis(0.45f);

    // 座標更新してからでなければローカルデータが消えてしまう
    modelTrans_.SetParent(&transform);
    modelTrans_.translate = {};
    modelTrans_.UpdateMatrix();
    playerModel_.Initialize(&modelTrans_);
    playerModel_.PlayAnimation(PlayerModel::kWait, true);
    //modelTrans_.scale = {1.0f,0.5f,1.0f};

    collider_ = std::make_unique<SphereCollider>();
    collider_->SetGameObject(this);
    collider_->SetName("Player");
    collider_->SetCenter(transform.translate + colliderOffset_);
    collider_->SetRadius(0.5f);
    collider_->SetCollisionAttribute(CollisionConfig::Player);
    collider_->SetCollisionMask(~CollisionConfig::Player);
    //collider_->SetSize({ 1.0f, 2.0f, 1.0f });
    collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });

    upperCollider_ = std::make_unique<SphereCollider>();
    upperCollider_->SetGameObject(this);
    upperCollider_->SetName("Player");
    upperCollider_->SetCenter(transform.translate + upperColliderOffset_);
    upperCollider_->SetRadius(0.5f);
    upperCollider_->SetCollisionAttribute(CollisionConfig::Player);
    upperCollider_->SetCollisionMask(~CollisionConfig::Player);

    // パラメーター初期化
    moveSpeed_ = 0.15f;
    jumpParameters_.isJumped = false;
    jumpParameters_.fallSpeed = 0.0f;
    jumpParameters_.jumpPower = 0.5f;

    floorCollider_ = nullptr;
    isCleared_ = false;

    shadowOffset_ = { 0.0f, 100.0f, 0.0f };
    circleShadow_ = CircleShadow::Create();
    circleShadow_->distance = 200.0f;
    circleShadow_->decay = 1.0f;
    circleShadow_->angle = 0.5f * Math::ToRadian;
    circleShadow_->falloffStartAngle = 0.45f * Math::ToRadian;
}

void Player::Update() {
#ifdef _DEBUG
    DrawImGui();
    if (Input::GetInstance()->IsKeyTrigger(DIK_R)) {
        transform.translate = Vector3::zero;
    }
#endif // DEBUG

    if (!jumpParameters_.isJumped &&
        playerModel_.GetAnimationType() != PlayerModel::kLanding && playerModel_.GetAnimationType() != PlayerModel::kBlend) {
        MoveUpdate();
    }
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

    isCollision_ = false;
}

void Player::PostCollisionUpdate() {
    // 球コライダーが衝突していなかったら
    if (!isCollision_ && !jumpParameters_.isJumped) {
        auto collisionManager = CollisionManager::GetInstance();
        Vector3 colliderPoint = colliderOffset_ * transform.worldMatrix;
        auto nearestInfo = collisionManager->NearestCollider(colliderPoint, CollisionConfig::Stage);
        if (nearestInfo.collider) {
            transform.translate = nearestInfo.point;

            Vector3 up = transform.rotate.GetUp();
            Vector3 normal = nearestInfo.normal.Normalized();
            if (std::abs(Dot(up, normal)) < 0.9999f) {
                Quaternion diff = Quaternion::MakeFromTwoVector(up, normal);
                transform.rotate = diff * transform.rotate;
            }
        }

        UpdateTransform();
    }

    if (moveDirection_ != Vector3::zero) {

        Vector3 lookDir = transform.rotate.Conjugate() * moveDirection_;
        if (Cross(lookDir, Vector3::up) != Vector3::zero) {
            modelTrans_.rotate = Quaternion::Slerp(0.1f, modelTrans_.rotate, Quaternion::MakeLookRotation(lookDir));
        }
    }

    modelTrans_.UpdateMatrix();
    playerModel_.Update();
}

void Player::SimpleUpdate() {
    // 座標更新
    transform.UpdateMatrix();
    modelTrans_.UpdateMatrix();
    // モデル座標更新
    playerModel_.Update();
}

void Player::UpdateTransform() {
    // 座標更新
    transform.UpdateMatrix();
    modelTrans_.UpdateMatrix();

    collider_->SetCenter(colliderOffset_ * transform.worldMatrix);
    upperCollider_->SetCenter(upperColliderOffset_ * transform.worldMatrix);

    circleShadow_->position = shadowOffset_ * transform.worldMatrix;
    circleShadow_->direction = -transform.rotate.GetUp();
    //collider_->SetOrientation(rotate);
}

void Player::MoveUpdate() {

    auto input = Input::GetInstance();
    Vector3 move;

    // キーボードでの移動
    if (input->IsKeyPressed(DIK_W)) {
        move.y += moveSpeed_;
    }
    if (input->IsKeyPressed(DIK_S)) {
        move.y -= moveSpeed_;
    }
    if (input->IsKeyPressed(DIK_A)) {
        move.x -= moveSpeed_;
    }
    if (input->IsKeyPressed(DIK_D)) {
        move.x += moveSpeed_;
    }

    auto& xinput = input->GetXInputState();
    if (std::abs(xinput.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        move.x = 0.0f;
        move.x = xinput.Gamepad.sThumbLX;
    }
    if (std::abs(xinput.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        move.y = 0.0f;
        move.y = xinput.Gamepad.sThumbLY;
    }

    // 移動処理
    if (move != Vector3::zero) {
        move = move.Normalized();

        auto& camera = camera_->GetCamera();

        auto viewProjInv = camera->GetViewProjectionMatrix().Inverse();
        // ndcからワールド空間に変換
        move = viewProjInv.ApplyTransformWDivide(move) - viewProjInv.ApplyTransformWDivide(Vector3::zero);

        move = Quaternion::MakeFromTwoVector(-camera->GetForward(), transform.rotate.GetUp()) * move;

        //move = camera_->GetCamera()->GetRotate() * move;
        move = move.Normalized();
        transform.translate += move * moveSpeed_;

        //Vector3 forward = transform.rotate.GetForward();
        //transform.rotate = transform.rotate * Quaternion::Slerp(0.1f, Quaternion::identity,Quaternion::MakeFromTwoVector(forward, move));

        auto animeType = playerModel_.GetAnimationType();
        if (animeType != PlayerModel::kWalk && animeType != PlayerModel::kBlend) {
            playerModel_.PlayAnimation(PlayerModel::kWalk, true);
        }
    }
    else {
        if (playerModel_.GetAnimationType() == PlayerModel::kWalk) {
            playerModel_.PlayAnimation(PlayerModel::kWait, true, true, 30);
        }
    }

    moveDirection_ = move;
}

void Player::MoveLimit() {
    auto input = Input::GetInstance();
    auto& xInput = input->GetXInputState();
    auto& preXInput = input->GetPreXInputState();
    if (input->IsKeyTrigger(DIK_R) ||
        xInput.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && !(preXInput.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
        jumpParameters_.isJumped = false;
        transform.translate = respawnPos_;
        transform.rotate = respawnRot_;
        jumpParameters_.fallSpeed = 0.0f;
        playerModel_.PlayAnimation(PlayerModel::kWait, false);
    }
}

void Player::JumpUpdate() {
    auto input = Input::GetInstance();
    auto& xInput = input->GetXInputState();
    auto& preXInput = input->GetPreXInputState();

    Vector3 gravityDirection = transform.rotate.GetUp();

    auto animeType = playerModel_.GetAnimationType();
    if (!jumpParameters_.isJumped &&
        (input->IsKeyTrigger(DIK_SPACE) ||
            xInput.Gamepad.wButtons & XINPUT_GAMEPAD_A && !(preXInput.Gamepad.wButtons & XINPUT_GAMEPAD_A)) &&
        animeType != PlayerModel::kLanding && animeType != PlayerModel::kBlend) {
        jumpParameters_.isJumped = true;
        jumpParameters_.fallSpeed = jumpParameters_.jumpPower;
        jumpParameters_.direction = moveDirection_;
        jumpParameters_.jumpHeight = Vector3::Dot(transform.translate, gravityDirection);

        playerModel_.PlayAnimation(PlayerModel::kJump, false, true, 30);
        fallCount_ = 0u;
    }

    jumpParameters_.fallSpeed -= jumpParameters_.gravity;
    transform.translate += gravityDirection * jumpParameters_.fallSpeed;

    if (jumpParameters_.isJumped &&
        Vector3::Dot(transform.translate, gravityDirection) > jumpParameters_.jumpHeight) {
        transform.translate += jumpParameters_.direction * moveSpeed_;
    }
    else if(jumpParameters_.isJumped) {
        FallTimer();
    }

    jumpParameters_.fallSpeed = std::max(jumpParameters_.fallSpeed, -jumpParameters_.fallSpeedLimits);
}

void Player::WallUpdate(Vector3 moveVec) {
    moveVec;
}

void Player::OnCollision(const CollisionInfo& collisionInfo) {
    if (collisionInfo.collider->GetName() == "Box") {
        // ワールド空間の押し出しベクトル
        Vector3 pushVector = collisionInfo.normal * collisionInfo.depth;
        transform.translate += pushVector;

        isCollision_ = true;

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

        if (jumpParameters_.isJumped) {
            Vector3 up = transform.rotate.GetUp();
            Vector3 normal = collisionInfo.normal.Normalized();

            if (std::abs(Dot(up, normal)) > std::cos(45.0f * Math::ToRadian)) {
                Landing();
            }
        }
        else {
            // 事前に飛ばしたレイに当たったコライダーは処理を通さない
            auto wallCollider = std::find(wallColliders_.begin(), wallColliders_.end(), collisionInfo.collider);
            if (wallCollider == wallColliders_.end()) {

                Landing();

                Vector3 up = transform.rotate.GetUp();
                Vector3 normal = collisionInfo.normal.Normalized();
                if (std::abs(Dot(up, normal)) < 0.9999f) {
                    Quaternion diff = Quaternion::MakeFromTwoVector(up, normal);
                    transform.rotate = diff * transform.rotate;
                }
            }
        }
        //}
    }

    if (collisionInfo.collider->GetName() == "Goal") {
        isCleared_ = true;
    }

    UpdateTransform();

}

void Player::DrawImGui() {
#ifdef _DEBUG

    ImGui::Begin("test", nullptr, ImGuiWindowFlags_MenuBar);
    ImGui::DragFloat("MoveSpeed", &moveSpeed_, 0.01f);
    ImGui::DragFloat("JumpPower", &jumpParameters_.jumpPower, 0.01f);
    ImGui::DragFloat("Gravity", &jumpParameters_.gravity, 0.01f);
    ImGui::DragFloat("FallSpeedLimits", &jumpParameters_.fallSpeedLimits, 0.01f);
    ImGui::DragFloat("FallSpeed", &jumpParameters_.fallSpeed, 0.01f, 0.0f, jumpParameters_.fallSpeedLimits);
    if (ImGui::Button("Reset")) {
        PlayerReset();
    }
    ImGui::End();

#endif // _DEBUG
}

void Player::Landing() {
    jumpParameters_.fallSpeed = 0.0f;
    if (jumpParameters_.isJumped) {
        jumpParameters_.isJumped = false;
        playerModel_.PlayAnimation(PlayerModel::kLanding, false, true, 20);
    }
}

void Player::FallTimer() {
    fallCount_++;
    if (fallCount_ >= kMaxCount_) {
        //Landing();
        PlayerReset();
    }
}

void Player::PlayerReset() {
    jumpParameters_.isJumped = false;
    transform.translate = respawnPos_;
    transform.rotate = respawnRot_;
    jumpParameters_.fallSpeed = 0.0f;
}
