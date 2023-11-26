#pragma once

#include "Math/Camera.h"
#include "Core/Graphics.h"
#include "Core/SwapChain.h"
#include "Core/CommandContext.h"
#include "Core/ColorBuffer.h"
#include "Core/DepthBuffer.h"
#include "Bloom.h"
#include "Renderer.h"
#include "PostEffect.h"
#include "Timer.h"
#include "SpriteRenderer.h"

class RenderManager {
public:

    static RenderManager* GetInstance();

    void Initialize();
    void Finalize();
    void Render();

    void SetCamera(const std::shared_ptr<Camera>& camera) { camera_ = camera; }

    ModelRenderer& GetModelRenderer() { return renderer_; }

    void UseGaussianBlur(bool useGaussianBlur) { useGaussianBlur_ = useGaussianBlur; }
    GaussianBlur& GetGaussianBlur() { return gaussianBlur_; }

private:
    RenderManager() = default;
    RenderManager(const RenderManager&) = delete;
    RenderManager& operator=(const RenderManager&) = delete;

    Graphics* graphics_ = nullptr;
    SwapChain swapChain_;
    CommandContext commandContexts_[SwapChain::kNumBuffers];

    ColorBuffer mainColorBuffer_;
    DepthBuffer mainDepthBuffer_;

    ModelRenderer renderer_;
    SpriteRenderer spriteRenderer_;
    GaussianBlur gaussianBlur_;
    Bloom bloom_;
    PostEffect postEffect_;
    
    Timer timer_;
    std::shared_ptr<const Camera> camera_;
    
    bool useGaussianBlur_;
};