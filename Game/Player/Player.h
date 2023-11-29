#pragma once
#include "Collision/GameObject.h"
#include "Graphics/Model.h"

#include <memory>
#include <map>

#include "Math/Transform.h"
#include "CameraAnimation/CameraAnimation.h"
#include "Collision/Collider.h"
#include "Graphics/LightManager.h"
#include "PlayerModel.h"

class Player : public GameObject {
public:
    // 初期化処理
    void Initialize();
    // 更新処理
    void Update();
    void PreCollisionUpdate();
    void PostCollisionUpdate();

    // エディター用
    void SimpleUpdate();

public: // ゲッター
    Transform GetTransform() { return transform; }
    bool GetCleared() { return isCleared_; }

public: // セッター
    void SetCamera(const std::shared_ptr<CameraAnimation>& camera) { camera_ = camera; }
    void SetRespawnPos(const Vector3& position) { respawnPos_ = position; }
    void SetRespawnRot(const Quaternion& rotation) { respawnRot_ = rotation; }

private: // メンバ変数
    // モデル
    std::shared_ptr<CameraAnimation> camera_;

    Transform modelTrans_;
    PlayerModel playerModel_;

    std::unique_ptr<SphereCollider> collider_;
    std::unique_ptr<SphereCollider> upperCollider_;
    Collider* floorCollider_;
    Vector3 colliderOffset_ = { 0.0f,0.5f,0.0f };
    Vector3 upperColliderOffset_ = { 0.0f,1.5f,0.0f };

    float moveSpeed_ = 0.0f;

    struct JumpParameters {
        bool isJumped = false; /*true している false していない*/
        float fallSpeed = 0.0f;
        float jumpPower = 0.2f;
        float gravity = 0.04f;
        float fallSpeedLimits = 0.4f;
        Vector3 direction;
        float jumpHeight;
    };
    JumpParameters jumpParameters_;

    Vector3 dotUp_;

    Vector3 moveDirection_;
    std::vector<Collider*> wallColliders_;
    bool isCollision_;

    bool isCleared_ = false;

    Vector3 respawnPos_;
    Quaternion respawnRot_;

    std::shared_ptr<CircleShadow> circleShadow_;
    Vector3 shadowOffset_;

private: // メンバ関数
    // 座標更新
    void UpdateTransform();
    // 移動
    void MoveUpdate();
    // 移動制限
    void MoveLimit();
    // ジャンプ
    void JumpUpdate();

    void Landing();

    void WallUpdate(Vector3 moveVec);

    void OnCollision(const CollisionInfo& collisionInfo);

    void DrawImGui();
};