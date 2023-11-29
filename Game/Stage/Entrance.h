#pragma once
#include "Collision/GameObject.h"

#include <memory>

#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Collision/Collider.h"
#include "Math/Color.h"

// ステージセレクト用の入口
class Entrance : public GameObject {
public:
    ~Entrance() {}

    void Initialize(const int& number);
    void Update();


    void DrawImGui();

    int stageNumber_ = 0;

private:
    std::unique_ptr<ModelInstance> wellModel_;
    std::unique_ptr<ModelInstance> boardModel_;
    std::unique_ptr<BoxCollider> collider_;

    // 回転保管用
    Vector3 rotate_;
    Vector3 color_ = Vector3(1.0f, 1.0f, 1.0f);

    bool isAlive_ = true;

    enum class AnimationType {
        Normal,
        Get,
    };
    AnimationType animationType_ = AnimationType::Normal;

private:
    void OnCollision(const CollisionInfo& collisionInfo);

    void AnimationUpdate();

};