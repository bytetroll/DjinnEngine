//
// Created by Dev on 4/5/2018.
//

#include <Base/Application.h>
#include <Data/IAssetCompiler.h>
#include <Data/IAssetHost.h>
#include <Platform/FileStream.h>
#include <Data/MaterialConfiguration.h>
#include <Data/IMaterialAsset.h>
#include <Data/IMaterialLayout.h>
#include <Data/IMaterialType.h>
#include <Data/IMaterialCompiler.h>

DJINN_NS();

class TestLayout : public Data::IMaterialLayout {
public:
    DJINN_COM_CLASS();

    TestLayout(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMaterialLayout(registry, outer, host) {

    }

    virtual Core::String GetPath() override {
        return "../Data/TestMaterialLayout.dsf";
    }
};

class TestType : public Data::IMaterialType {
public:
    DJINN_COM_CLASS();

    TestType(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMaterialType(registry, outer, host) {

    }

    virtual Core::String GetHeaderPath() override {
        return "../Data/TestMaterialHeader.dsf";
    }

    virtual Core::String GetKernelPath() override {
        return "../Data/TestMaterialKernel.dsf";
    }
};

void _main(Base::Application &app) {
    // Load plugins
    Always(app.GetRegistry()->LoadPlugins());

    TestLayout layout(app.GetRegistry(), nullptr, nullptr);
    TestType type(app.GetRegistry(), nullptr, nullptr);

    Data::IMaterialCompiler* materialCompiler;
    Always(app.GetRegistry()->GetPipeline()->GetInterface(&materialCompiler));

    Data::IAssetHost* assetHost;
    Always(app.GetRegistry()->GetPipeline()->GetInterface(&assetHost));

    materialCompiler->Register(&layout);
    materialCompiler->Register(&type);

    Data::MaterialConfiguration config;

    Data::IMaterialAsset *asset;
    for (UInt32 i = 0; i < 32; i++) {
        Platform::FileStream stream(Platform::FileMode::eRead, "../Data/Test.dmf");
        Always(stream.IsGood());

        Always(assetHost->CompileAsset(&stream, &config, &asset));
    }
}

int main() {
    Base::Launch(Base::ApplicationInfo().Name("Material Test"), Bindg(_main));
    return 0;
}