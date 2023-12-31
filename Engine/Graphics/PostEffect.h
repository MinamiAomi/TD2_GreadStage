#pragma once

#include "Core/RootSignature.h"
#include "Core/PipelineState.h"
#include "Core/ColorBuffer.h"

class CommandContext;

class PostEffect {
public:

    void Initialize(const ColorBuffer& target);
    void Render(CommandContext& commandContext, ColorBuffer& texture);


private:
    RootSignature rootSignature_;
    PipelineState pipelineState_;

    ColorBuffer* sourceTexture_;
};