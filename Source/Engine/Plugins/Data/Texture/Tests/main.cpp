//
// Created by Dev on 1/28/2018.
//

#include <Base/Application.h>
#include <Platform/FileStream.h>
#include <Data/ITextureCompiler.h>
#include <Data/IAssetHost.h>
#include <Data/ITextureAsset.h>
#include <Core/Timer.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Data);

void Main(Base::Application& app) {
    // Load plugins
    Always(app.GetRegistry()->LoadPlugin("Plugins.Data.Asset", Host::LoadFlag::eAll));
    Always(app.GetRegistry()->LoadPlugin("Plugins.Data.Texture", Host::LoadFlag::eAll));

    ITextureCompiler* compiler;
    Always(app.GetRegistry()->GetPipeline()->GetInterface(&compiler));

    IAssetHost* assetHost;
    Always(app.GetRegistry()->GetPipeline()->GetInterface(&assetHost));

    Platform::FileStream stream(Platform::FileMode::eRead, "../Data/diffuse.jpg");

    TextureConfiguration infoUncompressed;
    infoUncompressed.sourceFormat = TextureSourceFormat::eJpg;
    infoUncompressed.compressionMode = TextureCompressionMode::eNone;

    ITextureAsset *assetUncompressed;
    Always(compiler->Compile(&stream, &infoUncompressed, reinterpret_cast<IAsset**>(&assetUncompressed)));

    // Save asset
    Platform::FileStream outUncompressed(Platform::FileMode::eWrite, "testAssetUncompressed.dat");
    Always(assetHost->StoreAssetStream(&outUncompressed, assetUncompressed));

    TextureConfiguration infoCompressed;
    infoCompressed.sourceFormat = TextureSourceFormat::eJpg;
    infoCompressed.mode = TextureCompileMode::eInline;

    for (UInt32 i = 1; i <= static_cast<int>(TextureCompressionMode::eASTC); i++) {
        infoCompressed.compressionMode = static_cast<TextureCompressionMode>(i);

        ITextureAsset *assetCompressed;
        Always(compiler->Compile(&stream, &infoCompressed, reinterpret_cast<IAsset**>(assetCompressed)));

        const char *name = "";
        switch (static_cast<TextureCompressionMode>(i)) {
            default:
                break;
            case TextureCompressionMode::eBC1:
                name = "BC1";
                break;
            case TextureCompressionMode::eBC2:
                name = "BC2";
                break;
            case TextureCompressionMode::eBC3:
                name = "BC3";
                break;
            case TextureCompressionMode::eBC4:
                name = "BC4";
                break;
            case TextureCompressionMode::eBC5:
                name = "BC5";
                break;
            case TextureCompressionMode::eBC7:
                name = "BC7";
                break;
            case TextureCompressionMode::eASTC:
                name = "ASTC";
                break;
        }

        // Save asset
        Platform::FileStream outCompressed(Platform::FileMode::eWrite, Core::String("testAssetCompressed_") + name + ".dat");
        Always(assetHost->StoreAssetStream(&outCompressed, assetCompressed));

        InfoLog(app.GetRegistry()).Write(
                "Compression delta [", name, "]: ",
                (static_cast<double>(outUncompressed.GetSize()) / static_cast<double>(outCompressed.GetSize())) * 100.0, "% : ",
                (static_cast<double>(outUncompressed.GetSize()) / static_cast<double>(outCompressed.GetSize())), "x"
        );

        outCompressed.Close();
    }

    outUncompressed.Close();

    // Load asset
    ITextureAsset *dest;
    Platform::FileStream inStream(Platform::FileMode::eRead, "testAssetUncompressed.dat");
    Always(assetHost->LoadAsset(&inStream, reinterpret_cast<IAsset**>(&dest)));
    inStream.Close();
}

int main() {
    // Create application
    Base::Launch(Base::ApplicationInfo(), Bindg(Main));

    return 0;
}


