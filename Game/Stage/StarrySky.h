#pragma once
#include "Graphics/Renderable.h"

#include <vector>

#include "Math/MathUtils.h"
#include "Math/Random.h"
#include "Math/Camera.h"
#include "Graphics/Core/UploadBuffer.h"
#include "Graphics/Core/RootSignature.h"
#include "Graphics/Core/PipelineState.h"

class StarrySky :
    public Renderable {
public:
    struct Star {
        Matrix4x4 worldMatrix;
        float seed = 0.0f;
    };

    void Initialize();
    void Update();
    void Regenerate();

    void SetVolume(float volume) { volume_ = volume; }
    void SetCamera(const std::shared_ptr<const Camera>& camera) { camera_ = camera; }

    void OnRender(CommandContext& commandContext);

private:
    uint32_t numHorizontalGrids_ = 32;
    uint32_t numVerticalGrids_ = 32;
    uint32_t animationCycle_ = 300;
    float minScale_ = 1.0f;
    float maxScale_ = 5.0f;
    float distance_ = 200.0f;

    std::vector<Star> stars_;
    Random::RandomNumberGenerator randGen_;
    float animationParameter_ = 0.0f;
    float volume_ = 0.0f;
    std::shared_ptr<const Camera> camera_;
    RootSignature rootSignature_;
    PipelineState pipelineState_;
    UploadBuffer instanceBuffer_;
    UploadBuffer vertexBuffer_;
};