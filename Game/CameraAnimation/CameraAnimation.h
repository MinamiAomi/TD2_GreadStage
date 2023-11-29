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
    // title用更新
    void TitleUpdate();
private:
    void DrawImGui();
    void TransUpdate();
    void UpdateInput();

    void NormalUpdate();
    void StageMoveUpdate();

public: // ゲッター
    const std::shared_ptr<Camera>& GetCamera() { return camera_; }

public: //	セッター
    void SetTarget(const Transform* target) { target_ = target; }
    void SetCamera();
    void SetTitleMove(const bool& flag) { isLeaved_ = flag; }

private: //	メンバ変数
    std::shared_ptr<Camera> camera_;
    Vector3 offset_;
    Vector3 radRot_;
    const Transform* target_ = nullptr;

    // ローカルの角度(pitch, yaw)
    Vector2 angles_;
    Vector3 destinationTranslate_;
    Quaternion destinationRotate_;
    float followDelay_ = 0.95f;
    float distance_ = 15.0f;

    // 度数法(pitch, yaw)
    Vector2 cameraSpeed_ = { 2.0f,2.0f };
    // pitch角の制限(up, down)
    Vector2 pitchAngleLimits_ = { 60.0f, -50.0f };

    Vector3 destinationTargetPosition_;
    Vector3 lastTargetPosition_;

    Vector2 preAngles_;

    bool isLeaved_ = true;
    bool preIsLeaved_ = true;
    bool isTitleMove_ = false;

    Vector2 upDown = Vector2::zero;

    // ステージのゴールからのカメラ移動用フラグ
    bool isStageMove_ = false;

    // イージング用
    Vector3 startPos_;
    Vector3 endPos_;
    Vector2 startAngle_;
    Vector2 endAngle_;
    float easeCount_ = 0.0f;
    void EaseUpdate();

public:
    static bool nowTitle_;

};
