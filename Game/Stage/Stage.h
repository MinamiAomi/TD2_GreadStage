#pragma once

#include <memory>
#include <vector>
#include <filesystem>

#include "Box/Box.h"
#include "Goal/Goal.h"
#include "Player/Player.h"
#include "RequiredItem.h"
#include "CollectionObject.h"
#include "Graphics/LightManager.h"

class Stage {
public:
    void Initialize();
    void Update();

    void Add(const std::shared_ptr<Box>& box);
    void Add(const std::shared_ptr<RequiredItem>& item);
    void Add(const std::shared_ptr<CollectionObject>& collect);
    void DeleteBox(const int& num);
    void DeleteItem(const int& num);
    void DeleteCollect(const int& num);

    void Load(const std::filesystem::path& loadFile);

    const std::vector<std::shared_ptr<Box>>& GetBoxes() const { return boxes_; }
    const std::vector<std::shared_ptr<RequiredItem>>& GetItems() const { return items_; }
    const std::vector<std::shared_ptr<CollectionObject>>& GetCollects() const { return collects_; }
    const std::shared_ptr<Goal>& GetGoal() const { return goal_; }
    const std::shared_ptr<Player>& GetPlayer() const { return player_; }
    void SetPlayerPtr(std::shared_ptr<Player> player) { player_ = player; }

private:
    std::vector<std::shared_ptr<Box>> boxes_;
    std::vector<std::shared_ptr<RequiredItem>> items_;
    std::vector<std::shared_ptr<CollectionObject>> collects_;
    std::shared_ptr<Goal> goal_;
    std::shared_ptr<Player> player_;
    std::shared_ptr<DirectionalLight> light_;
};