#pragma once

#include <memory>
#include <vector>

#include "Floor/Floor.h"
#include "Wall/Wall.h"

class Stage {
public:
    void Initialize();
    void Update();

private:
    std::vector<std::shared_ptr<Floor>> floors_;
    std::vector<std::shared_ptr<Wall>> walls_;
};