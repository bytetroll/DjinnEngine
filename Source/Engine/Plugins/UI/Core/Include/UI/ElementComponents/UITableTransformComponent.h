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
 on 8/20/2018.
//
#pragma once

#include <Core/HashMap.h>
#include <Core/List.h>
#include "IUITransformComponent.h"

namespace Djinn::UI {
    class DJINN_EXPORT_PLUGINS_UI_CORE UITableTransformComponent : public IUITransformComponent {
    public:
        DJINN_COM_CLASS();

        UITableTransformComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Configure this table
        /// \param rows
        /// \param columns
        void Configure(UInt32 rowCount, UInt32 columnCount);

        /// Get a cell
        /// \param row
        /// \param column
        /// \return
        void SetCell(UInt32 row, UInt32 column, IUIElement* element);

        /// Overrides
        Math::Vector2 Measure() override;
        void Arrange() override;
        IUITransformComponent *TraceChild(const Math::Vector2 &point, TransformTraceMode mode) override;
        IUITransformComponent *FindChild(const Core::StringView &name) override;

    protected:
        /// Overrides
        bool OnChildDetached(IUITransformComponent *child) override;
        void OnDirty() override;

    public:
        /// Set padding
        /// \param padding
        void SetPadding(const Math::Vector4 &padding) {
            this->padding = padding;
            MarkAsDirty();
        }

        /// Get padding
        /// \return
        const Math::Vector4 &GetPadding() const {
            return padding;
        }

        /// Set padding
        /// \param padding
        void SetCellPadding(const Math::Vector2 &padding) {
            this->cellPadding = padding;
            MarkAsDirty();
        }

        /// Get padding
        /// \return
        const Math::Vector2 &GetCellPadding() const {
            return cellPadding;
        }

        /// Set the row wise scaling
        /// \param row
        /// \param scaling
        void SetRowScaling(UInt32 row, UIScaling scaling) {
            if (scaling == UIScaling::eContent) {
                SetChildDependent(true);
            }
            rowScaling[row].mode = scaling;
        }

        /// Set the column wise scaling
        /// \param column
        /// \param scaling
        void SetColumnScaling(UInt32 column, UIScaling scaling) {
            if (scaling == UIScaling::eContent) {
                SetChildDependent(true);
            }
            columnScaling[column].mode = scaling;
        }

        /// Get the row wise scaling
        /// \param row
        /// \return
        UIScaling GetRowScaling(UInt32 row) const {
            return rowScaling[row].mode;
        }

        /// Get the column wise scaling
        /// \param column
        /// \return
        UIScaling GetColumnScaling(UInt32 column) const {
            return columnScaling[column].mode;
        }

        /// Set the row height
        /// \param row
        /// \param scaling
        void SetRowHeight(UInt32 row, float height) {
            rowScaling[row].height = height;
        }

        /// Set the column width
        /// \param column
        /// \param scaling
        void SetColumnWidth(UInt32 column, float width) {
            columnScaling[column].width = width;
        }

        /// Get the row wise scaling
        /// \param row
        /// \return
        float GetRowHeight(UInt32 row) const {
            return rowScaling[row].height;
        }

        /// Get the column wise scaling
        /// \param column
        /// \return
        float GetColumnWidth(UInt32 column) const {
            return columnScaling[column].width;
        }

    protected:
        /// Padding of this ui component (Only applies to children)
        /* ╔═══════════════════════════════════════════════════════╗
         * ║  Com       ║                                  ║
         * ║            ║ <- padding.y                     ║
         * ║ ========== ╔══════════════════════════╗           ║
         * ║  ^         ║                      ║           ║
         * ║  padding.x ║   Child Component    ║           ║
         * ║            ║                      ║           ║
         * ║            ╚══════════════════════════╝ ========= ║
         * ║                      padding.w -> ║  ^        ║
         * ║                                   ║  padding.z║
         * ╚═══════════════════════════════════════════════════════╝
         * */
        Math::Vector4 padding;

        Math::Vector2 cellPadding = {3, 3};

        /// Row wise scaling
        struct RowScaling {
            UIScaling mode;
            float height = 35;
            float size = 0;
            float margin = 0;
        };
        Core::Array<RowScaling> rowScaling;

        /// Column wise scaling
        struct ColumnScaling {
            UIScaling mode;
            float width = 35;
            float size = 0;
            float margin = 0;
        };
        Core::Array<ColumnScaling> columnScaling;

        struct Row {
            /// Cells
            Core::Array<Core::WeakPtr<IUIElement>> cells;
        };

        Math::Vector2 contentSize;

        /// All rows
        Core::Array<Row> rows;
    };
}