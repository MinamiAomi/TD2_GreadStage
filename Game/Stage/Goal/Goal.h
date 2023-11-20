#pragma once
#include "Engine/Collision/GameObject.h"

#include <memory>

#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Collision/Collider.h"

class Goal : public GameObject {
public:
	Goal() = default;
	~Goal() = default;

	void Initialize();
	void Update();

private:
	std::unique_ptr<ModelInstance> model_;
	std::unique_ptr<BoxCollider> collider_;

};