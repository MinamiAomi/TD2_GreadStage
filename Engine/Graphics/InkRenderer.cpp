#include "InkRenderer.h"

#include "Core/CommandContext.h"
#include "Core/ColorBuffer.h"
#include "Core/Helper.h"
#include "Core/ShaderManager.h"

wchar_t kDrawInkVertexShader[] = L"Engine/Graphics/Shader/DrawInkVS.hlsl";
wchar_t kDrawInkGeometryShader[] = L"Engine/Graphics/Shader/DrawInkGS.hlsl";

void InkRenderer::Initialize(const ColorBuffer& colorBuffer) {

    CD3DX12_ROOT_PARAMETER rootParameter[2]{};
    rootParameter[0].InitAsConstantBufferView(0);
    rootParameter[1].InitAsShaderResourceView(0);

    D3D12_ROOT_SIGNATURE_DESC rsDesc{};
    rsDesc.pParameters = rootParameter;
    rsDesc.NumParameters = 2;
    rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    rootSignature_.Create(L"InkRenderer RootSignature", rsDesc);

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psDesc{};

    psDesc.pRootSignature = rootSignature_;

    D3D12_INPUT_ELEMENT_DESC ieDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "WIDTH"   , 0, DXGI_FORMAT_R32_FLOAT      , 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    D3D12_INPUT_LAYOUT_DESC ilDesc{};
    ilDesc.pInputElementDescs = ieDesc;
    ilDesc.NumElements = _countof(ieDesc);
    psDesc.InputLayout = ilDesc;

    ShaderManager* shaderManager = ShaderManager::GetInstance();
    auto vs = shaderManager->Compile(kDrawInkVertexShader, ShaderManager::kVertex);
    auto gs = shaderManager->Compile(kDrawInkGeometryShader, ShaderManager::kGeometry);
    psDesc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBufferPointer(), vs->GetBufferSize());
    psDesc.GS = CD3DX12_SHADER_BYTECODE(gs->GetBufferPointer(), gs->GetBufferSize());

    psDesc.RasterizerState = Helper::RasterizerDefault;
    psDesc.DepthStencilState = Helper::DepthStateReadWrite;
    psDesc.DSVFormat = depthBuffer_.GetFormat();
    psDesc.NumRenderTargets = 2;
    psDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
    psDesc.SampleDesc = { .Count = 1, .Quality = 0 };
    psDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

    pipelineState_.Create(L"InkRenderer PipelineState", psDesc);

    depthBuffer_.CreateArray(L"InkRenderer DepthBuffer", colorBuffer.GetWidth(), colorBuffer.GetHeight(), 2, DXGI_FORMAT_D32_FLOAT);
}

void InkRenderer::Render(CommandContext& commandContext, const Camera& camera) {
    
    struct Constant {
        Matrix4x4 viewProjectionMatrix;
        uint32_t numVertices;
    };

    commandContext.TransitionResource(depthBuffer_, D3D12_RESOURCE_STATE_DEPTH_WRITE);
    commandContext.SetRenderTargets(0, nullptr, depthBuffer_.GetDSV());
    commandContext.ClearDepth(depthBuffer_);
    commandContext.SetViewportAndScissorRect(0, 0, depthBuffer_.GetWidth(), depthBuffer_.GetHeight());

    commandContext.SetRootSignature(rootSignature_);
    commandContext.SetPipelineState(pipelineState_);
    commandContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

    Constant constant{};
    constant.viewProjectionMatrix = camera.GetViewProjectionMatrix();
    constant.numVertices = 4;
    commandContext.SetDynamicConstantBufferView(0, sizeof(Constant), &constant);

    std::vector<Vector3> vertices {
        {  1.0f,  1.0f, 0.0f },
        {  1.0f, -1.0f, 0.0f },
        { -1.0f, -1.0f, 0.0f },
        { -1.0f,  1.0f, 0.0f },
    };
    commandContext.SetDynamicShaderResourceView(0, sizeof(vertices[0]) * vertices.size(), vertices.data());
    
    commandContext.SetDynamicVertexBuffer(0, lines_.size(), sizeof(lines_[0]), lines_.data());
    commandContext.Draw(UINT(lines_.size()));

    commandContext.TransitionResource(depthBuffer_, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}
