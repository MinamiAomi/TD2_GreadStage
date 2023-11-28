#include "LightManager.h"

std::list<DirectionalLight*> DirectionalLight::instanceList_;
std::list<CircleShadow*> CircleShadow::instanceList_;

std::shared_ptr<DirectionalLight> DirectionalLight::Create() {
    struct Helper : public DirectionalLight {
        Helper() : DirectionalLight() {}
    };

    std::shared_ptr<DirectionalLight> p = std::make_shared<Helper>();

    return p;
}

DirectionalLight::DirectionalLight() {
    instanceList_.push_back(this);
}

DirectionalLight::~DirectionalLight() {
    std::erase(instanceList_, this);
}

std::shared_ptr<CircleShadow> CircleShadow::Create() {
    struct Helper : public CircleShadow {
        Helper() : CircleShadow() {}
    };

    std::shared_ptr<CircleShadow> p = std::make_shared<Helper>();

    return p;
}

CircleShadow::CircleShadow() {
    instanceList_.push_back(this);
}

CircleShadow::~CircleShadow() {
    std::erase(instanceList_, this);
}
