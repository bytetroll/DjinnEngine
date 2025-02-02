//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 11/12/2018.
//
#pragma once

#include <Math/Vector4.h>
#include <Math/Matrix4.h>
#include <Graphics/UIBrushPacket.h>
#include <Graphics/UIGeometryPacket.h>
#include <Core/StringView.h>
#include <Core/Array.h>
#include "IntermediateCache.h"

namespace Djinn::Graphics {
    class UIDrawPacketBuffer;
}

namespace Djinn::UI {
    class IUITransformComponent;

    enum class ImmediatePositionMode {
        eNone,
        eCenter
    };

    class DJINN_EXPORT_PLUGINS_UI_CONTROLS ImmediateBuilder {
    public:
        /// Construct this immediate builder
        /// \param transform
        /// \param buffer
        ImmediateBuilder(IUITransformComponent* transform, Graphics::UIDrawPacketBuffer* buffer);

        /// Set viewport
        /// \param viewport
        void SetViewport(const Math::Vector4& viewport);

        /// Set viewport
        /// \param viewport
        void SetLocalViewport(const Math::Vector2& offset, const Math::Vector2& size);

        /// Reset the current local viewport
        void ResetLocalViewport();

        /// Set transform
        /// \param transform
        void SetTransform(const Math::Matrix4& transform);

        /// Set brush
        /// \param packet
        void SetBrush(const Graphics::UIBrushPacket& packet);

        /// Set font
        /// \param packet
        void SetFont(const Data::AssetView<Data::IFontAsset>& font);

        /// Set position mode
        /// \param mode
        void SetPositionMode(ImmediatePositionMode mode, const Math::Vector2& container = {0, 0});

        /// Set the depth offset
        /// \param offset
        void SetDepthOffset(int offset);

        /// Draw box
        /// \param margin
        /// \param size
        void Box(const Math::Vector2& offset, const Math::Vector2& size);

        /// Draw box
        /// \param margin
        /// \param size
        void MarginBox(const Math::Vector2& offset, const Math::Vector2& size, const Math::Vector2& margin);

        /// Draw line
        /// \param start
        /// \param end
        void Line(const Graphics::UIGeometryVertex& start, const Graphics::UIGeometryVertex& end, float width);

        /// Draw lines
        /// \param margin
        /// \param size
        void Lines(const Core::ArrayView<Graphics::UIGeometryVertex>& vertices, float width, bool connected);

        /// Draw a label
        /// \param str
        void Label(IntermediateCache& cache, const Math::Vector2& offset, const Core::StringView& str, const Math::Vector4& color = {1, 1, 1, 1}, float smoothing = 2.0f);

    private:
        Math::Vector2 GetOffset(const Math::Vector2& offset, const Math::Vector2& size);

        /// Current position mode
        ImmediatePositionMode mode = ImmediatePositionMode::eNone;
        Math::Vector2 positionContainer = {0, 0};

        /// Current viewport
        Math::Vector4 absoluteViewport = {-1e6f, -1e6f, 1e6f, 1e6f};
        Math::Vector4 viewport = {-1e6f, -1e6f, 1e6f, 1e6f};

        /// Current transform
        Math::Matrix4 transform = Math::Matrix4::Identity();

        /// Current brush
        Graphics::UIBrushPacket brush = Graphics::UIBrushSolid({1, 1, 1, 1});

        /// Current font
        Data::AssetView<Data::IFontAsset> font = nullptr;

        /// Current draw buffer
        Graphics::UIDrawPacketBuffer* buffer = nullptr;

        /// Current depth offset
        int depthOffset = 0;
    };
}