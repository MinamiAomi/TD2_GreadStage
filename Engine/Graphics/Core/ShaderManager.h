#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl/client.h>

#include <string>
#include <vector>
#include <filesystem>

class ShaderManager {
public:
    static ShaderManager* GetInstance();

    enum Type {
        kVertex,
        kPixel,
        kGeometry,
        kHull,
        kDomain,
        kCompute,

        kNumTypes
    };

    void Initialize();
    void SetDirectory(const std::filesystem::path& directory) { directory_ = directory; }
    Microsoft::WRL::ComPtr<IDxcBlob> Compile(const std::filesystem::path& path, Type type);

private:
    ShaderManager() = default;
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;

    Microsoft::WRL::ComPtr<IDxcBlob> Compile(const std::wstring& path, const std::wstring& profile);

    Microsoft::WRL::ComPtr<IDxcUtils> utils_;
    Microsoft::WRL::ComPtr<IDxcCompiler3> compiler_;
    Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_;

    std::vector<std::wstring> loadedShaderList_;

    std::filesystem::path directory_;
};