#include "StarrySky.h"

#include <numeric>
#include <numbers>

#include "Graphics/Core/CommandContext.h"
#include "Graphics/Core/ShaderManager.h"
#include "Graphics/RenderManager.h"

struct StarVertex {
    Vector3 position;
    Vector2 texcoord;
};
void StarrySky::Initialize() {

    CD3DX12_ROOT_PARAMETER rootParamters[2]{};
    rootParamters[0].InitAsShaderResourceView(0);
    rootParamters[1].InitAsConstantBufferView(0);

    D3D12_ROOT_SIGNATURE_DESC rsDesc{};
    rsDesc.NumParameters = _countof(rootParamters);
    rsDesc.pParameters = rootParamters;
    rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    rootSignature_.Create(L"StarrySky RootSignature", rsDesc);

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psDesc{};

    psDesc.pRootSignature = rootSignature_;

    auto shaderManager = ShaderManager::GetInstance();
    auto vs = shaderManager->Compile(L"Engine/Graphics/Shader/StarrySkyVS.hlsl", ShaderManager::kVertex);
    auto ps = shaderManager->Compile(L"Engine/Graphics/Shader/StarrySkyPS.hlsl", ShaderManager::kPixel);
    psDesc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBufferPointer(), vs->GetBufferSize());
    psDesc.PS = CD3DX12_SHADER_BYTECODE(ps->GetBufferPointer(), ps->GetBufferSize());

    D3D12_INPUT_ELEMENT_DESC ies[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    };
    D3D12_INPUT_LAYOUT_DESC ilDesc{};
    ilDesc.NumElements = _countof(ies);
    ilDesc.pInputElementDescs = ies;
    psDesc.InputLayout = ilDesc;

    psDesc.BlendState = Helper::BlendDisable;
    psDesc.DepthStencilState = Helper::DepthStateReadWrite;
    psDesc.RasterizerState = Helper::RasterizerNoCull;

    auto& colorBuffer = RenderManager::GetInstance()->GetMainColorBuffer();
    auto& depthBuffer = RenderManager::GetInstance()->GetMainDepthBuffer();
    // 前面カリング
    psDesc.NumRenderTargets = 1;
    psDesc.RTVFormats[0] = colorBuffer.GetRTVFormat();
    psDesc.DSVFormat = depthBuffer.GetFormat();
    psDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    psDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psDesc.SampleDesc.Count = 1;
    pipelineState_.Create(L"StarrySky PipelineState", psDesc);

    StarVertex vertices[] = {
        { {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f }},
        { {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f }},
        { { -0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f }},

        { {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f }},
        { { -0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f }},
        { { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }},
    };

    vertexBuffer_.Create(L"StarrySky VertexBuffer", _countof(vertices), sizeof(vertices[0]));
    vertexBuffer_.Copy(vertices, sizeof(vertices));

    Regenerate();
}

void StarrySky::Update() {
    animationParameter_ += 1.0f / animationCycle_;
    animationParameter_ = animationParameter_ - float(int(animationParameter_));
}

void StarrySky::Regenerate() {
    stars_.resize(size_t(numHorizontalGrids_ * numVerticalGrids_));

    // 被りなし乱数を生成
    std::vector<int> seeds(stars_.size());
    std::iota(seeds.begin(), seeds.end(), 0);
    std::shuffle(seeds.begin(), seeds.end(), randGen_.GetEngine());


    auto SphereSurface = [](float theta, float phi) {
        float ct = std::cos(theta), st = std::sin(theta);
        float cp = std::cos(phi), sp = std::sin(phi);
        st;
        Vector3 point = { ct * cp, st, ct * sp };
        return point;
    };

    float deltaTheta = std::numbers::pi_v<float> / (numVerticalGrids_ + 1);
    float deltaPhi = 2.0f * std::numbers::pi_v<float> / numHorizontalGrids_;

    float reciSize = 1.0f / seeds.size();
    for (size_t y = 0; y < numVerticalGrids_; ++y) {
        for (size_t x = 0; x < numHorizontalGrids_; ++x) {
            size_t index = y * numHorizontalGrids_ + x;
            auto& star = stars_[index];

            star.seed = seeds[index] * reciSize;

            float theta = -std::numbers::pi_v<float> / 2.0f + deltaTheta * y;
            float phi = deltaPhi * x;
            phi += Math::Lerp(randGen_.NextFloatUnit(), 0.0f, deltaTheta);
            theta += Math::Lerp(randGen_.NextFloatUnit(), 0.0f, deltaPhi);
            Vector3 position = SphereSurface(theta, phi) * (distance_ + star.seed * 10.0f);

            Vector3 up = Vector3::unitY;
            if (position.x == 0.0f && position.z == 0.0f) {
                up = Vector3::unitX;
            }
            star.worldMatrix = Matrix4x4::identity;
            star.worldMatrix *= Matrix4x4::MakeScaling(Vector3(Math::Lerp(1.0f, minScale_, maxScale_)));
            star.worldMatrix *= Matrix4x4::MakeRotationZ(star.seed * Math::TwoPi);
            star.worldMatrix *= Matrix4x4::MakeLookRotation(-position, up);
            star.worldMatrix *= Matrix4x4::MakeTranslation(position);
        }
    }

    std::sort(stars_.begin(), stars_.end(), [](const Star& a, const Star& b) {return a.seed < b.seed; });

    instanceBuffer_.Create(L"StarrySky InstanceBuffer", stars_.size(), sizeof(stars_[0]));
    instanceBuffer_.Copy(stars_.data(), instanceBuffer_.GetBufferSize());
}

void StarrySky::OnRender(CommandContext& commandContext) {
    if (volume_ <= 0.0f ||
        !camera_) {
        return;
    }

    struct CommonCB {
        Matrix4x4 viewProjectionMatrix;
        Vector3 cameraPosition;
        float time;
    };

    commandContext.SetRootSignature(rootSignature_);
    commandContext.SetPipelineState(pipelineState_);
    commandContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    commandContext.TrackingObject(instanceBuffer_.Get());
    commandContext.SetShaderResource(0, instanceBuffer_.GetGPUVirtualAddress());

    CommonCB commonCB{};
    commonCB.viewProjectionMatrix = camera_->GetViewProjectionMatrix();
    commonCB.cameraPosition = camera_->GetPosition();
    commonCB.time = animationParameter_;
    commandContext.SetDynamicConstantBufferView(1, sizeof(commonCB), &commonCB);

    D3D12_VERTEX_BUFFER_VIEW vbv{};
    vbv.BufferLocation = vertexBuffer_.GetGPUVirtualAddress();
    vbv.SizeInBytes = UINT(vertexBuffer_.GetBufferSize());
    vbv.StrideInBytes = UINT(sizeof(StarVertex));
    commandContext.SetVertexBuffer(0, vbv);


    UINT drawCount = UINT(volume_ * stars_.size());
    commandContext.DrawInstanced(6, drawCount);
}

