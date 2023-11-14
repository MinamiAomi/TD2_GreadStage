#pragma once

#include "Core/RootSignature.h"
#include "Core/PipelineState.h"
#include "Core/TextureResource.h"

class CommandContext;
class ColorBuffer;
class DepthBuffer;
class Camera;

struct ModelRootIndex {
    enum Index {
        Scene = 0,
        Instance,
        Material,
        Texture,
        Sampler,
        DirectionalLight,

        NumParameters
    };
};

class ModelRenderer {
public:

    void Initialize(const ColorBuffer& colorBuffer, const DepthBuffer& depthBuffer);
    void Render(CommandContext& commandContext, const Camera& camera);

private:
    void InitializeRootSignature();
    void InitializePipelineState(DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat);

    RootSignature rootSignature_;
    PipelineState pipelineState_;
};