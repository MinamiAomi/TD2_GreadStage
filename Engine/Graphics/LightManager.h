#pragma once

#include <memory>
#include <list>

#include "Math/MathUtils.h"

class DirectionalLight {
public:
    static std::shared_ptr<DirectionalLight> Create();
    static std::list<DirectionalLight*>& GetInstanceList() { return instanceList_; }

    DirectionalLight(const DirectionalLight&) = delete;
    DirectionalLight& operator=(const DirectionalLight&) = delete;
    DirectionalLight(DirectionalLight&&) = delete;
    DirectionalLight& operator=(DirectionalLight&&) = delete;

    Vector3 direction = -Vector3::unitY;
    Vector3 color = Vector3::one;
    float intensity = 1.0f;
    bool isActive = true;

private:
    DirectionalLight();
    ~DirectionalLight();

    static std::list<DirectionalLight*> instanceList_;

};

class CircleShadow {
public:
    static std::shared_ptr<CircleShadow> Create();
    static std::list<CircleShadow*>& GetInstanceList() { return instanceList_; }


    CircleShadow(const CircleShadow&) = delete;
    CircleShadow& operator=(const CircleShadow&) = delete;
    CircleShadow(CircleShadow&&) = delete;
    CircleShadow& operator=(CircleShadow&&) = delete;

    Vector3 position;
    float distance = 10.0f;
    Vector3 direction = Vector3::down;
    float decay = 2.0f;
    float angle = Math::Pi / 3.0f;
    float falloffStartAngle = Math::Pi / 4.0f;
    bool isActive = true;

private:
    CircleShadow();
    ~CircleShadow();

    static std::list<CircleShadow*> instanceList_;

};