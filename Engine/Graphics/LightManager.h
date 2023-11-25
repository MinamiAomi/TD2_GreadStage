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
