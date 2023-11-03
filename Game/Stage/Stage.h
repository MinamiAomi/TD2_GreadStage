#pragma once

#include <memory>
#include <vector>

#include "Ground/Ground.h"

class Stage {
public:
    void Initialize();
    void Update();

private:
    std::shared_ptr<Ground> ground_;
};