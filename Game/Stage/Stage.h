#pragma once

#include <memory>
#include <vector>
#include <filesystem>

#include "Box/Box.h"

class Stage {
public:
    void Initialize();
    void Update();

    void Add(const std::shared_ptr<Box>& box);
    void Delete(const int& num);
    void Load(const std::filesystem::path& loadFile);

    const std::vector<std::shared_ptr<Box>>& GetBoxes() const { return boxes_; }

private:
    std::vector<std::shared_ptr<Box>> boxes_;
};