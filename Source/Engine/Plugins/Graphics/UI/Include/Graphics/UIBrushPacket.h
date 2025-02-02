//
// Created by Dev on 2/27/2018.
//
#pragma once

#include <Core/Types.h>
#include <Math/Vector4.h>
#include <GHA/SamplerDesc.h>
#include <Data/Asset.h>

namespace Djinn::Data {
    class ITextureAsset;
}

namespace Djinn::GHA {
    class ITexture;
}

namespace Djinn::Graphics {
    class IOutput;

    enum class UIBrushType : UInt8 {
        eNone,
        eSolid,
        eGradient,
        eGHATexture,
        eOutputTexture,
        eTexture,
        eSDF
    };

    struct UIBrushSolid {
        UIBrushSolid() = default;

        UIBrushSolid(const Math::Vector4& color) : color(color) {

        }

        UIBrushType type = UIBrushType::eSolid;
        Math::Vector4 color;
    };

    struct UIBrushGradient {
        UIBrushGradient() = default;

        UIBrushGradient(const Math::Vector2& begin, const Math::Vector4& beginColor, const Math::Vector2& end, const Math::Vector4& endColor)
                : begin(begin), beginColor(beginColor), end(end), endColor(endColor) {

        }

        UIBrushType type = UIBrushType::eGradient;
        Math::Vector2 begin;
        Math::Vector4 beginColor;
        Math::Vector2 end;
        Math::Vector4 endColor;
    };

    struct UIBrushTexture {
        UIBrushTexture() = default;

        UIBrushTexture(const Math::Vector4& color, const Data::AssetView<Data::ITextureAsset>& asset, const GHA::SamplerDesc& sampler) : color(color), asset(asset), sampler(sampler) {

        }

        UIBrushType type = UIBrushType::eTexture;
        Data::AssetView<Data::ITextureAsset> asset;
        Math::Vector4 color;
        GHA::SamplerDesc sampler;
    };

    struct UIBrushGHATexture {
        UIBrushGHATexture() = default;

        UIBrushGHATexture(const Math::Vector4& color, GHA::ITexture* texture, const GHA::SamplerDesc& sampler) : color(color), texture(texture), sampler(sampler) {

        }

        UIBrushType type = UIBrushType::eGHATexture;
        GHA::ITexture* texture = nullptr;
        Math::Vector4 color;
        GHA::SamplerDesc sampler;
    };

    struct UIBrushOutputTexture {
        UIBrushOutputTexture() = default;

        UIBrushOutputTexture(const Math::Vector4& color, IOutput* output, const GHA::SamplerDesc& sampler) : color(color), output(output), sampler(sampler) {

        }

        UIBrushType type = UIBrushType::eOutputTexture;
        IOutput* output = nullptr;
        Math::Vector4 color;
        GHA::SamplerDesc sampler;
    };

    struct UIBrushSDF {
        UIBrushSDF() = default;

        UIBrushSDF(const Math::Vector4& color, float smoothing) : color(color), smoothing(smoothing) {

        }

        UIBrushType type = UIBrushType::eSDF;
        Math::Vector4 color;
        float smoothing = 1.0f / 16.0f;
    };

    union UIBrushPacket {
        UIBrushPacket() : type(UIBrushType::eNone) {
            // ...
        }

        UIBrushPacket(const UIBrushSolid &solid) : solid(solid) {
            // ...
        }

        UIBrushPacket(const UIBrushGradient &gradient) : gradient(gradient) {
            // ...
        }

        UIBrushPacket(const UIBrushTexture &texture) : texture(texture) {
            // ...
        }

        UIBrushPacket(const UIBrushGHATexture &ghaTexture) : ghaTexture(ghaTexture) {
            // ...
        }

        UIBrushPacket(const UIBrushOutputTexture &outputTexture) : outputTexture(outputTexture) {
            // ...
        }

        UIBrushPacket(const UIBrushSDF &sdf) : sdf(sdf) {
            // ...
        }

        UIBrushType type;
        UIBrushSolid solid;
        UIBrushGradient gradient;
        UIBrushTexture texture;
        UIBrushGHATexture ghaTexture;
        UIBrushOutputTexture outputTexture;
        UIBrushSDF sdf;
    };

}