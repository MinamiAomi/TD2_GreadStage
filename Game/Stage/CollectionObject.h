#pragma once
#include "Collision/GameObject.h"

#include <memory>

#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Collision/Collider.h"
#include "Math/Color.h"

// クリスタルとかの拾得物
class CollectionObject : public GameObject {
public:
    ~CollectionObject() {}

    void Initialize();
    void Update();


    void DrawImGui();

    bool GetIsAlived() { return isAlive_; }

private:
    std::unique_ptr<ModelInstance> model_;
    std::unique_ptr<BoxCollider> collider_;

    // 回転保管用
    Vector3 rotate_;
    Vector3 color_ = Vector3::one;

    bool isAlive_ = true;

    enum class AnimationType {
        Normal,
        Get,
    };
    AnimationType animationType_ = AnimationType::Normal;
    size_t soundHandle_;
    size_t playHandle_;

private:
    void OnCollision(const CollisionInfo& collisionInfo);
    
    void AnimationUpdate();
    void TypeNormalUpdate();
    void TypeGetUpdate();


};