#include "Renderer.h"

#include "Core/Helper.h"
#include "Core/ShaderManager.h"
#include "Core/ColorBuffer.h"
#include "Core/DepthBuffer.h"
#include "Core/CommandContext.h"
#include "Math/Camera.h"
#include "Model.h"
#include "Core/SamplerManager.h"
#include "DefaultTextures.h"

const wchar_t kModelVertexShadedr[] = L"Engine/Graphics/Shader/ModelVS.hlsl";
const wchar_t kModelPixelShadedr[] = L"Engine/Graphics/Shader/ModelPS.hlsl";

void ModelRenderer::Initialize(const ColorBuffer& colorBuffer, const DepthBuffer& depthBuffer) {
    InitializeRootSignature();
    InitializePipelineState(colorBuffer.GetRTVFormat(), depthBuffer.GetFormat());
}

void ModelRenderer::Render(CommandContext& commandContext, const Camera& camera) {

    struct SceneConstant {
        Matrix4x4 viewMatrix;
        Matrix4x4 projectionMatrix;
        Vector3 cameraPosition;
        float ditheringRange;
    };

    struct InstanceConstant {
        Matrix4x4 worldMatrix;
        Vector3 color;
        uint32_t useLighting{};
    };

    auto& instanceList = ModelInstance::GetInstanceList();


    // 描画
    commandContext.SetRootSignature(rootSignature_);
    commandContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    SceneConstant scene{};
    scene.viewMatrix = camera.GetViewMatrix();
    scene.projectionMatrix = camera.GetProjectionMatrix();
    scene.cameraPosition = camera.GetPosition();
    scene.ditheringRange = ditheringRange_;
    commandContext.SetDynamicConstantBufferView(ModelRootIndex::Scene, sizeof(scene), &scene);

    for (auto& instance : instanceList) {
        if (instance->IsActive() && instance->model_) {

            InstanceConstant data;
            data.worldMatrix = instance->worldMatrix_;
            data.color = instance->color_;
            data.useLighting = instance->useLighting_ ? 1 : 0;
            commandContext.SetDynamicConstantBufferView(ModelRootIndex::Instance, sizeof(data), &data); 

            // オブジェクト描画
            commandContext.SetPipelineState(pipelineState_);
            for (auto& mesh : instance->model_->meshes_) {
                commandContext.SetConstantBuffer(ModelRootIndex::Material, mesh.material->constantBuffer.GetGPUVirtualAddress());
                if (mesh.material->texture) {
                    commandContext.SetDescriptorTable(ModelRootIndex::Texture, mesh.material->texture->GetSRV());
                }
                else {
                    commandContext.SetDescriptorTable(ModelRootIndex::Texture, DefaultTexture::White.GetSRV());
                }
                commandContext.SetDescriptorTable(ModelRootIndex::Sampler, SamplerManager::AnisotropicWrap);

                D3D12_VERTEX_BUFFER_VIEW vbv{};
                vbv.BufferLocation = mesh.vertexBuffer.GetGPUVirtualAddress();
                vbv.SizeInBytes = (UINT)mesh.vertexBuffer.GetBufferSize();
                vbv.StrideInBytes = (UINT)sizeof(ModelData::Vertex);
                commandContext.SetVertexBuffer(0, vbv);
                D3D12_INDEX_BUFFER_VIEW ibv{};
                ibv.BufferLocation = mesh.indexBuffer.GetGPUVirtualAddress();
                ibv.SizeInBytes = (UINT)mesh.indexBuffer.GetBufferSize();
                ibv.Format = (sizeof(ModelData::Index) == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
                commandContext.SetIndexBuffer(ibv);
                commandContext.DrawIndexed(mesh.indexCount);
            }

        }
    }
}

void ModelRenderer::InitializeRootSignature() {
    CD3DX12_DESCRIPTOR_RANGE srvRange{};
    srvRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    CD3DX12_DESCRIPTOR_RANGE samplerRange{};
    samplerRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);

    CD3DX12_ROOT_PARAMETER rootParameters[ModelRootIndex::NumParameters]{};
    rootParameters[ModelRootIndex::Scene].InitAsConstantBufferView(0);
    rootParameters[ModelRootIndex::Instance].InitAsConstantBufferView(1);
    rootParameters[ModelRootIndex::Material].InitAsConstantBufferView(2);
    rootParameters[ModelRootIndex::Texture].InitAsDescriptorTable(1, &srvRange);
    rootParameters[ModelRootIndex::Sampler].InitAsDescriptorTable(1, &samplerRange);
    rootParameters[ModelRootIndex::DirectionalLight].InitAsConstantBufferView(3);

    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.NumParameters = _countof(rootParameters);
    rootSignatureDesc.pParameters = rootParameters;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignature_.Create(L"Model RootSignature", rootSignatureDesc);
}

void ModelRenderer::InitializePipelineState(DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat) {

    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{};

    pipelineStateDesc.pRootSignature = rootSignature_;

    D3D12_INPUT_ELEMENT_DESC inputElements[] = {
         { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
         { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
         { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };
    D3D12_INPUT_LAYOUT_DESC inputLayout{};
    inputLayout.NumElements = _countof(inputElements);
    inputLayout.pInputElementDescs = inputElements;
    pipelineStateDesc.InputLayout = inputLayout;

    auto shaderManager = ShaderManager::GetInstance();
    auto vs = shaderManager->Compile(kModelVertexShadedr, ShaderManager::kVertex);
    auto ps = shaderManager->Compile(kModelPixelShadedr, ShaderManager::kPixel);

    pipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBufferPointer(), vs->GetBufferSize());
    pipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(ps->GetBufferPointer(), ps->GetBufferSize());

    pipelineStateDesc.BlendState = Helper::BlendDisable;
    pipelineStateDesc.DepthStencilState = Helper::DepthStateReadWrite;
    pipelineStateDesc.RasterizerState = Helper::RasterizerDefault;
    // 前面カリング
    pipelineStateDesc.NumRenderTargets = 1;
    pipelineStateDesc.RTVFormats[0] = rtvFormat;
    pipelineStateDesc.DSVFormat = dsvFormat;
    pipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pipelineStateDesc.SampleDesc.Count = 1;
    pipelineState_.Create(L"Model PipelineState", pipelineStateDesc);
}
