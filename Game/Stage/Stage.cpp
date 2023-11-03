#include "Stage.h"

struct FloorInitData {
    Vector3 base;
    float rotate;
};

static const FloorInitData stageInits[] = {
	{Vector3(0.0f,0.0f,0.0f),0.0f},
	{Vector3(-5.0f,3.0f,0.0f),90.0f * Math::ToRadian},
};

void Stage::Initialize() {
    floors_.resize(_countof(stageInits));
    uint32_t i = 0;
    for (auto& floor : floors_) {
        floor = std::make_shared<Floor>();
        floor->Initialize(stageInits[i].base, stageInits[i].rotate);
        i++;
    }
}

void Stage::Update() {

    for (auto& floor : floors_) {
        floor->Update();
    }
}
