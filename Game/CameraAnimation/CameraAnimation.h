#pragma once
#include "Collision/GameObject.h"

#include "Math/Camera.h"
#include "Math/Transform.h"
#include <memory>

class CameraAnimation :
    public GameObject {
public:
    void Initialize();
    void Update();
    void Restart();
private:
    void DrawImGui();
    void TransUpdate();
    void UpdateInput();


public: // ゲッター
    const std::shared_ptr<Camera>& GetCamera() { return camera_; }

public: //	セッター
    void SetTarget(const Transform* target) { target_ = target; }
    void SetCamera();

private: //	メンバ変数
    std::shared_ptr<Camera> camera_;
    Vector3 offset_;
    Vector3 radRot_;
    const Transform* target_ = nullptr;

    // ローカルの角度(pitch, yaw)
    Vector2 angles_;
    Vector3 destinationTranslate_;
    Quaternion destinationRotate_;
    float followDelay_ = 0.9f;
    float distance_ = 15.0f;

    // 度数法(pitch, yaw)
    Vector2 cameraSpeed_ = { 1.0f,1.0f };
    // pitch角の制限(up, down)
    Vector2 pitchAngleLimits_ = { 60.0f, -50.0f };



    Vector2 upDown = Vector2::zero;

};
