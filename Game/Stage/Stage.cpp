#include "Stage.h"

struct BoxInitData {
    Vector3 base;
    float rotate;
};

static const BoxInitData floorInits[] = {
	{Vector3(0.0f,0.0f, 0.0f),0.0f},
	{Vector3(0.0f,0.0f,-5.0f),0.0f},
	{Vector3(0.0f,0.0f,-10.0f),0.0f},
	{Vector3(0.0f,0.0f,-15.0f),0.0f},
	{Vector3(0.0f,0.0f,-20.0f),0.0f},
    {Vector3(5.0f,0.0f, 0.0f),0.0f},
	{Vector3(5.0f,0.0f,-5.0f),0.0f},
	{Vector3(5.0f,0.0f,-10.0f),0.0f},
	{Vector3(5.0f,0.0f,-15.0f),0.0f},
	{Vector3(5.0f,0.0f,-20.0f),0.0f},
};

static const BoxInitData wallInits[] = {
	{Vector3(-10.0f,2.0f,-5.0f),90.0f * Math::ToRadian},
	{Vector3(-5.0f,2.0f,-5.0f),90.0f * Math::ToRadian},
	{Vector3(0.0f,2.0f,-5.0f),90.0f * Math::ToRadian},
	{Vector3(5.0f,2.0f,-5.0f),90.0f * Math::ToRadian},
	{Vector3(10.0f,2.0f,-5.0f),90.0f * Math::ToRadian},
};

void Stage::Initialize() {
    floors_.resize(_countof(floorInits));
    uint32_t i = 0;
    for (auto& floor : floors_) {
        floor = std::make_shared<Floor>();
        floor->Initialize(floorInits[i].base, floorInits[i].rotate);
        i++;
    }
    
    walls_.resize(_countof(wallInits));
    i = 0;
    for (auto& wall : walls_) {
        wall = std::make_shared<Wall>();
        wall->Initialize(wallInits[i].base, wallInits[i].rotate);
        i++;
    }
}

void Stage::Update() {

    for (auto& floor : floors_) {
        floor->Update();
    }
    for (auto& wall : walls_) {
        wall->Update();
    }
}
