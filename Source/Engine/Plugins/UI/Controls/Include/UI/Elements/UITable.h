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
 on 8/23/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <UI/ElementComponents/UITableTransformComponent.h>
#include <Core/Annotation.h>
#include <UI/UIMLAnnotation.h>

namespace Djinn::UI {
    class IUITransformComponent;
    class TableSeperator;

    ClassAttr(UIML::Class("table"))
    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UITable : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UITable(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;

    public:
        /// Configure this table
        /// \param rows
        /// \param columns
        void Configure(UInt32 rows, UInt32 columns);

        /// Add a child
        /// \param child
        //Attr(UIML::SocketAttach())
        void SetCell(UInt32 row, UInt32 column, IUITransformComponent *child);

        /// Remove a child
        /// \param child
        //Attr(UIML::SocketDetach())
        void RemoveCell(UInt32 row, UInt32 column);

        /// Set padding
        /// \param padding
        Attr(UIML::Setter("resizable"))
        void SetResizable(bool state);

        /// Get padding
        /// \return
        Attr(UIML::Getter("resizable"))
        bool IsResizable() const {
            return resizable;
        }

        /// Set padding
        /// \param padding
        void SetPadding(const Math::Vector4 &padding) {
            static_cast<UITableTransformComponent*>(GetTransform())->SetPadding(padding);
        }

        /// Get padding
        /// \return
        const Math::Vector4 &GetPadding() const {
            return static_cast<UITableTransformComponent*>(GetTransform())->GetPadding();
        }

        /// Set the row wise scaling
        /// \param row
        /// \param scaling
        void SetRowScaling(UInt32 row, UIScaling scaling) {
            static_cast<UITableTransformComponent*>(GetTransform())->SetRowScaling(row, scaling);
        }

        /// Set the column wise scaling
        /// \param column
        /// \param scaling
        void SetColumnScaling(UInt32 column, UIScaling scaling) {
            static_cast<UITableTransformComponent*>(GetTransform())->SetColumnScaling(column, scaling);
        }

        /// Get the row wise scaling
        /// \param row
        /// \return
        UIScaling GetRowScaling(UInt32 row) const {
            return static_cast<UITableTransformComponent*>(GetTransform())->GetRowScaling(row);
        }

        /// Get the column wise scaling
        /// \param column
        /// \return
        UIScaling GetColumnScaling(UInt32 column) const {
            return static_cast<UITableTransformComponent*>(GetTransform())->GetColumnScaling(column);
        }

        /// Set the row height
        /// \param row
        /// \param scaling
        void SetRowHeight(UInt32 row, float height) {
            static_cast<UITableTransformComponent*>(GetTransform())->SetRowHeight(row, height);
        }

        /// Set the column width
        /// \param column
        /// \param scaling
        void SetColumnWidth(UInt32 column, float width) {
            static_cast<UITableTransformComponent*>(GetTransform())->SetColumnWidth(column, width);
        }

        /// Get the row wise scaling
        /// \param row
        /// \return
        float GetRowHeight(UInt32 row) const {
            return static_cast<UITableTransformComponent*>(GetTransform())->GetRowHeight(row);
        }

        /// Get the column wise scaling
        /// \param column
        /// \return
        float GetColumnWidth(UInt32 column) const {
            return static_cast<UITableTransformComponent*>(GetTransform())->GetColumnWidth(column);
        }

    private:
        bool resizable = false;
    };
}