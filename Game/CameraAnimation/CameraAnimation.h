#pragma once
#include "Collision/GameObject.h"

#include "Math/Camera.h"
#include "Math/Transform.h"
#include <memory>

class CameraAnimation :
	public GameObject
{
public:
	void Initialize();
	void Update();
	void Restart();
private:
	void DrawImGui();


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

	Vector2 upDown = Vector2::zero;

};
