#include "LightManager.h"

std::list<DirectionalLight*> DirectionalLight::instanceList_;

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
