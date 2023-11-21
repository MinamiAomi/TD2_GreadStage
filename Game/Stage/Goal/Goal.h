#pragma once
#include "Engine/Collision/GameObject.h"

#include <memory>

#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Collision/Collider.h"

class Goal : public GameObject {
public:
	Goal() = default;
	~Goal() override = default;

	void Initialize();
	void Update();

	void DrawImGui();

private:
	std::unique_ptr<ModelInstance> model_;
	std::unique_ptr<BoxCollider> collider_;
	Vector3 rotate_;

};