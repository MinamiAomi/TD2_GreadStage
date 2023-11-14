#pragma once

#include <map>
#include <memory>
#include <string>

class Model;

class ResourceManager {
public:
    static ResourceManager* GetInstance();

    void AddToonModel(const std::string& name, const std::shared_ptr<Model>& model) { toonModelMap_.emplace(std::make_pair(name, model)); }
    std::shared_ptr<Model> FindModel(const std::string& name) const { return toonModelMap_.at(name); }

private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::map<std::string, std::shared_ptr<Model>> toonModelMap_;
};