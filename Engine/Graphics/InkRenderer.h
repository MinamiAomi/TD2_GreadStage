#pragma once

#include <vector>

#include "Core/RootSignature.h"
#include "Core/PipelineState.h"
#include "Core/DepthBuffer.h"
#include "Math/Camera.h"

class CommandContext;
class ColorBuffer;

class InkRenderer {
public:
    struct Point {
        Vector3 position;
        Vector3 normal;
        float width;
    };

    void Initialize(const ColorBuffer& colorBuffer);
    void Render(CommandContext& commandContext, const Camera& camera);

private:
    RootSignature rootSignature_;
    PipelineState pipelineState_;
    DepthBuffer depthBuffer_;
    std::vector<Point> lines_;
};