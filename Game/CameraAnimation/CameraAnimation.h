#pragma once
#include "Collision/GameObject.h"

#include "Math/Camera.h"
#include "Math/Transform.h"
#include <memory>
#include <optional>

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
    float distance_ = 16.5f;

    // 度数法(pitch, yaw)
    Vector2 cameraSpeed_ = { 1.5f,1.5f };
    // pitch角の制限(up, down)
    Vector2 pitchAngleLimits_ = { 60.0f, -50.0f };

    Vector3 destinationTargetPosition_;
    Vector3 lastTargetPosition_;

    Vector2 preAngles_;

    bool isLeaved_ = true;
    bool preIsLeaved_ = true;
    bool isTitleMove_ = false;

    // ステージのゴールからのカメラ移動用フラグ
    bool isStageMove_ = false;

    // イージング用

    struct Set {
        Vector3 pos;
        Vector2 angle;
        float speed;
    };
public:
    struct EaseParameter {
        Set first;
        Set second;
        Set third;
        Set fourth;
        Set fifth;
    };
private:
    EaseParameter easeParameter_;
    enum class Fase {
        First,
        Second,
        Third,
        Fourth,
        Fifth
    };
    Fase nowFase_ = Fase::First;
    //	次の振る舞いリクエスト
    std::optional<Fase> behaviorRequest_ = Fase::First;

    Set easeStart_;
    Set easeEnd_;
    
    float easeCount_ = 0.0f;
    float easeSpeed_ = 0.0f;
    bool EaseUpdate();

    bool isDuringReset_ = false;
    bool skipFlg_ = false;

public:
    void SetEaseParameter(const EaseParameter& param) { easeParameter_ = param; }
    bool GetStageMove() const { return isStageMove_; }
    static bool nowTitle_;
    void SetReseted(const bool& flag) { isDuringReset_ = flag; }

};
