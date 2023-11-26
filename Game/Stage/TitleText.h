#pragma once
#include "Collision/GameObject.h"

#include <memory>
#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Collision/Collider.h"

class TitleText : public GameObject {
public:
	TitleText() = default;
	~TitleText() = default;

    void Initialize();
	void Update();

    void DrawImGui();

private:
    std::unique_ptr<ModelInstance> model_;

    // 回転保管用
    Vector3 rotate_;

    bool isAlive_ = true;

    enum class AnimationType {
        Normal,
        Get,
    };
    AnimationType animationType_ = AnimationType::Normal;

private:

    void AnimationUpdate();

};

