//
// Created by Dev on 8/23/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <UI/ElementComponents/IUIRenderComponent.h>
#include <Graphics/UIGeometryPacket.h>
#include <Math/Vector4.h>
#include <Graphics/UIBrushPacket.h>
#include <Core/Optional.h>
#include <Core/Buffer.h>

namespace Djinn::UI {
    enum class UIPlotType {
        eLine,
        eLineFill,
        ePoint
    };

    struct UIPlotProperties {
        /// Brush to be applied
        Graphics::UIBrushPacket brush;

        /// Type of this plot
        UIPlotType type = UIPlotType::eLineFill;

        /// Brush size
        float brushSize = 2.0f;
    };

    struct UIPlotData {
        Core::Array<Math::Vector2> data;
    };

    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UIGraph : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UIGraph(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;
        bool Cache() override;

    public:
        /// Add a plot with given properties
        /// \param properties
        UInt32 AddPlot(const UIPlotProperties &_properties) {
            auto plot = renderer->plotPool.Pop();
            plot->properties = _properties;
            renderer->plots.Add(plot);
            return static_cast<UInt32>(renderer->plots.Size() - 1);
        }

        /// Remove plot at index
        /// \param index
        void RemovePlot(UInt32 index) {
            renderer->plotPool.Push(renderer->plots[index]);
            renderer->plots.Remove(index);
        }

        /// Set unit snap
        /// \param value
        void SetGridUnitSnap(float value) {
            renderer->gridUnitSnap = value;
        }

        /// Set the window texture
        /// \param texture
        void SetTexture(const Data::Asset<Data::ITextureAsset> &texture) {
            renderer->texture = texture;
        }

        /// Viewport
        /// \param viewport
        void SetViewport(const Core::Optional<Math::Vector2> &viewport) {
            renderer->viewport = viewport;
        }

        /// Get plot properties
        /// \param index
        /// \return
        UIPlotProperties GetPlotProperties(UInt32 index) {
            return renderer->plots[index]->properties;
        }

        /// Set plot properties
        /// \param index
        /// \param _properties
        void SetPlotProperties(UInt32 index, const UIPlotProperties &_properties) {
            renderer->plots[index]->properties = _properties;
        }

        /// Get plot data and lock it
        /// \param index
        /// \return
        UIPlotData *MapPlotData(UInt32 index) {
            renderer->mappedPlots++;
            return &renderer->plots[index]->data;
        }

        /// Unmap the plot data
        /// \param data
        void UnmapPlotData(UIPlotData *data) {
            if (--renderer->mappedPlots <= 0) {
                renderer->mappedPlots = 0;
                renderer->MarkAsInvalidated();
                MarkAsUncached();
            }
        }

        /// Get number of plots
        /// \return
        UInt32 GetPlotCount() {
            return static_cast<UInt32>(renderer->plots.Size());
        }

        /// Get grid unit snap
        /// \return
        float GetGridUnitSnap() {
            return renderer->gridUnitSnap;
        }

        /// Get texture
        /// \return
        Data::Asset<Data::ITextureAsset> GetTexture() {
            return renderer->texture;
        }

        /// Get viewport
        /// \return
        const Core::Optional<Math::Vector2> &GetViewport() {
            return renderer->viewport;
        }

    public:
        class Renderer : public IUIRenderComponent {
        public:
            DJINN_COM_CLASS();

            Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

            void Build(Graphics::UIDrawPacketBuffer *buffer) override;

            struct PlotData {
                UIPlotProperties properties;
                UIPlotData data;
                Core::Buffer<Graphics::UIGeometryVertex> cachedVertices;
                Core::Buffer<Graphics::UIGeometryIndex> cachedIndices;
            };

            /// Grid unit snap
            float gridUnitSnap = 5.0f;

            /// Graph viewport
            Core::Optional<Math::Vector2> viewport;

            /// Graph texture
            Data::Asset<Data::ITextureAsset> texture = nullptr;

            /// Plots
            Core::Array<PlotData *> plots;
            Core::ObjectPool<PlotData> plotPool;

            /// Packet cache
            Math::Vector2 cachedMax;
            Core::Array<PlotData *> cachedPlots;
            Core::Buffer<Graphics::UIGeometryVertex> cachedLines;

            /// Data mutex
            Platform::SpinMutex attribMutex;

            /// Current number of mapped plots
            Int32 mappedPlots = 0;
        };

    private:
        Renderer *renderer;
    };
}