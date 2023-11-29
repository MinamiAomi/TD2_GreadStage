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

    // sin派
    const float amplitude_ = 0.5f;  // 振幅
    const float frequency_ = 1.0f;  // 周波数（1サイクルあたりの波の数）
    const float phaseShift_ = 10.0f; // 位相シフト（初期位相）

    int nowTime_ = 0;

private:

    void AnimationUpdate();

};

