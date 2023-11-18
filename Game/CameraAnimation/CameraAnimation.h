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

public: // ゲッター
	const std::shared_ptr<Camera>& GetCamera() { return camera_; }

public: //	セッター
	void SetTarget(const Transform* target) { target_ = target; }

private: //	メンバ変数
	std::shared_ptr<Camera> camera_;
	Vector3 offset_;
	const Transform* target_ = nullptr;

};
