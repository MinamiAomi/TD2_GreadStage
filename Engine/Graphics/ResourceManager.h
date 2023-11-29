#pragma once

#include <map>
#include <memory>
#include <string>

class Model;
class Texture;

class ResourceManager {
public:
    static ResourceManager* GetInstance();

    void AddToonModel(const std::string& name, const std::shared_ptr<Model>& model) { toonModelMap_.emplace(std::make_pair(name, model)); }
    std::shared_ptr<Model> FindModel(const std::string& name) const { return toonModelMap_.at(name); }

    void AddTexture(const std::string& name, const std::shared_ptr<Texture>& texture) {
        textureMap_.emplace(std::make_pair(name, texture));
    }
    std::shared_ptr<Texture> FindTexture(const std::string& name) const { return textureMap_.at(name); }

    void AddSound(const std::string& name, size_t soundHandle) { 
        soundMap_.emplace(std::make_pair(name, soundHandle));
    }
    size_t FindSound(const std::string& name) const { return soundMap_.at(name); }

private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::map<std::string, std::shared_ptr<Model>> toonModelMap_;
    std::map<std::string, std::shared_ptr<Texture>> textureMap_;
    std::map<std::string, size_t> soundMap_;
};