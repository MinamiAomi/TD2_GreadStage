#include "RenderManager.h"

#include "Core/ShaderManager.h"
#include "DefaultTextures.h"
#include "GameWindow.h"
#include "ImGuiManager.h"

RenderManager* RenderManager::GetInstance() {
    static RenderManager instance;
    return &instance;
}

void RenderManager::Initialize() {
    graphics_ = Graphics::GetInstance();
    graphics_->Initialize();

    auto shaderManager = ShaderManager::GetInstance();
    shaderManager->Initialize();
    shaderManager->SetDirectory(std::filesystem::current_path() / SHADER_DIRECTORY);

    auto window = GameWindow::GetInstance();
    swapChain_.Create(window->GetHWND());

    for (auto& commandContext : commandContexts_) {
        commandContext.Create();
        commandContext.Close();
    }

    DefaultTexture::Initialize();

    auto& swapChainBuffer = swapChain_.GetColorBuffer();
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    mainColorBuffer_.SetClearColor(clearColor);
    mainColorBuffer_.Create(L"MainColorBuffer", swapChainBuffer.GetWidth(), swapChainBuffer.GetHeight(), DXGI_FORMAT_R8G8B8A8_UNORM);
    mainDepthBuffer_.Create(L"MainDepthBuffer", swapChainBuffer.GetWidth(), swapChainBuffer.GetHeight(), DXGI_FORMAT_D32_FLOAT);

    renderer_.Initialize(mainColorBuffer_, mainDepthBuffer_);

    useGaussianBlur_ = true;
    gaussianBlur_.Initialize(&mainColorBuffer_);
    bloom_.Initialize(&mainColorBuffer_);
    spriteRenderer_.Initialize(swapChainBuffer);
    postEffect_.Initialize(swapChainBuffer);

    timer_.Initialize();

    auto imguiManager = ImGuiManager::GetInstance();
    imguiManager->Initialize(window->GetHWND(), swapChainBuffer.GetRTVFormat());
    imguiManager->NewFrame();
}

void RenderManager::Finalize() {
    auto imguiManager = ImGuiManager::GetInstance();
    imguiManager->Shutdown();

    DefaultTexture::Finalize();

    graphics_->Finalize();
}

void RenderManager::Render() {
    auto& commandContext = commandContexts_[swapChain_.GetBufferIndex()];

    std::erase_if(customRenderers_, [](auto& p) {return p.expired(); });

    commandContext.Reset();

    commandContext.TransitionResource(mainColorBuffer_, D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandContext.TransitionResource(mainDepthBuffer_, D3D12_RESOURCE_STATE_DEPTH_WRITE);
    commandContext.SetRenderTarget(mainColorBuffer_.GetRTV(), mainDepthBuffer_.GetDSV());
    commandContext.ClearColor(mainColorBuffer_);
    commandContext.ClearDepth(mainDepthBuffer_);
    commandContext.SetViewportAndScissorRect(0, 0, mainColorBuffer_.GetWidth(), mainColorBuffer_.GetHeight());

    if (camera_) {
        renderer_.Render(commandContext, *camera_);
    }


    for (auto& p : customRenderers_) {
        auto ptr = p.lock();
        if (ptr) {
            ptr->OnRender(commandContext);
        }
    }

    if (useGaussianBlur_) {
        gaussianBlur_.Render(commandContext);
    }
   
    bloom_.Render(commandContext);

    auto& swapChainBuffer = swapChain_.GetColorBuffer();
    commandContext.TransitionResource(swapChainBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandContext.SetRenderTarget(swapChainBuffer.GetRTV());
    commandContext.ClearColor(swapChainBuffer);
    commandContext.SetViewportAndScissorRect(0, 0, swapChainBuffer.GetWidth(), swapChainBuffer.GetHeight());

    auto postEffectTargetTexture = &mainColorBuffer_;
    if (useGaussianBlur_) {
       postEffectTargetTexture =  &gaussianBlur_.GetResult();
    }
    
    postEffect_.Render(commandContext, *postEffectTargetTexture);
    spriteRenderer_.Render(commandContext, 0.0f, 0.0f, (float)swapChainBuffer.GetWidth(), (float)swapChainBuffer.GetHeight());

#ifdef _DEBUG
    //ImGui::Begin("Profile");
    //auto io = ImGui::GetIO();
    //ImGui::Text("Framerate : %f", io.Framerate);
    //ImGui::End();
#endif // _DEBUG

    // ImGuiを描画
    auto imguiManager = ImGuiManager::GetInstance();
    imguiManager->Render(commandContext);

    commandContext.TransitionResource(swapChainBuffer, D3D12_RESOURCE_STATE_PRESENT);
    commandContext.Close();
    CommandQueue& commandQueue = graphics_->GetCommandQueue();
    commandQueue.WaitForGPU();
    commandQueue.Excute(commandContext);
    swapChain_.Present();
    commandQueue.Signal();

    timer_.KeepFrameRate(60);

    imguiManager->NewFrame();
}

void RenderManager::AddCustomRenderer(const std::shared_ptr<Renderable>& renderer) {
    customRenderers_.emplace_back(renderer);
}
