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

#include <UI/ElementComponents/UITableTransformComponent.h>
#include <UI/IUIElement.h>

DJINN_NS2(UI);

UITableTransformComponent::UITableTransformComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUITransformComponent(registry, outer, host) {

}

Math::Vector2 UITableTransformComponent::Measure() {
    desiredSize = {};

    // Account for rows
    for (USize i = 0; i < rowScaling.Size(); i++) {
        auto &row = rowScaling[i];

        switch (rowScaling[i].mode) {
            case UIScaling::eNone: {
                row.size = row.height;
                break;
            }
            case UIScaling::eFill: {
                row.size = 0;
                break;
            }
            case UIScaling::eContent: {
                float max = 0;
                for (auto &&cell : rows[i].cells) {
                    if (cell) {
                        max = Math::Max(max, cell->GetTransform()->GetDesiredSize().y);
                    }
                }
                row.size = max;
                break;
            }
        }

        row.size += padding.y + padding.w;

        desiredSize.y += row.size + cellPadding.y;
    }

    // Account for columns
    for (USize i = 0; i < columnScaling.Size(); i++) {
        auto &column = columnScaling[i];

        switch (columnScaling[i].mode) {
            case UIScaling::eNone: {
                column.size = column.width;
                break;
            }
            case UIScaling::eFill: {
                column.size = 0;
                break;
            }
            case UIScaling::eContent: {
                float max = 0;
                for (USize row = 0; row < rows.Size(); row++) {
                    auto &&cell = rows[row].cells[i];
                    if (cell) {
                        max = Math::Max(max, cell->GetTransform()->GetDesiredSize().x);
                    }
                }
                column.size = max;
                break;
            }
        }

        column.size += padding.x + padding.z;

        desiredSize.x += column.size + cellPadding.x;
    }

    return contentSize = desiredSize;
}

void UITableTransformComponent::Arrange() {
    // Determine fill counts
    Math::Vector2 fillCount;
    for (auto &&scaling : rowScaling) {
        if (scaling.mode == UIScaling::eFill) {
            fillCount.y++;
        }
    }
    for (auto &&scaling : columnScaling) {
        if (scaling.mode == UIScaling::eFill) {
            fillCount.x++;
        }
    }

    // Determine fill size
    Math::Vector2 fillSize = (desiredSize - contentSize) / fillCount;
    fillSize.x = Math::Max(fillSize.x, 0);
    fillSize.y = Math::Max(fillSize.y, 0);

    // Apply fill sizes
    for (USize i = 0; i < rowScaling.Size(); i++) {
        auto &&scaling = rowScaling[i];

        if (scaling.mode == UIScaling::eFill) {
            scaling.size = fillSize.y;
        }

        if (i != 0) {
            scaling.margin = rowScaling[i - 1].margin + rowScaling[i - 1].size + cellPadding.y;
        }
    }
    for (USize i = 0; i < columnScaling.Size(); i++) {
        auto &&scaling = columnScaling[i];

        if (scaling.mode == UIScaling::eFill) {
            scaling.size = fillSize.x;
        }

        if (i != 0) {
            scaling.margin = columnScaling[i - 1].margin + columnScaling[i - 1].size + cellPadding.x;
        }
    }

    // Foreach cell
    for (USize rowIndex = 0; rowIndex < rows.Size(); rowIndex++) {
        auto &&row = rows[rowIndex];
        auto &&rowScale = rowScaling[rowIndex];

        for (USize columnIndex = 0; columnIndex < row.cells.Size(); columnIndex++) {
            auto &&cell = row.cells[columnIndex];
            auto &&columnScale = columnScaling[columnIndex];

            auto child = cell->GetTransform();

            Math::Vector2 adjustedMargin = child->GetMargin();
            Math::Vector2 adjustedPadding = {};

            // Account for horizontal alignment
            switch (child->GetHorizontalAlignment()) {
                case UIHorizontalAlignment::eLeft:
                    adjustedPadding.x = padding.x;
                    break;
                case UIHorizontalAlignment::eRight:
                    adjustedPadding.x = -padding.z - padding.x;
                    adjustedMargin.x = columnScale.size - child->GetDesiredSize().x - child->GetMargin().x;
                    break;
                case UIHorizontalAlignment::eCenter:
                    adjustedMargin.x = columnScale.size / 2.0f - child->GetDesiredSize().x / 2.0f + child->GetMargin().x;
                    break;
            }

            // Account for vertical alignment
            switch (child->GetVerticalAlignment()) {
                case UIVerticalAlignment::eTop:
                    adjustedPadding.y = padding.y;
                    break;
                case UIVerticalAlignment::eBottom:
                    adjustedPadding.y = -padding.w - padding.y;
                    adjustedMargin.y = rowScale.size - child->GetDesiredSize().y - child->GetMargin().y;
                    break;
                case UIVerticalAlignment::eCenter:
                    adjustedMargin.y = rowScale.size / 2.0f - child->GetDesiredSize().y / 2.0f + child->GetMargin().y;
                    break;
            }

            // ...
            auto desiredSize = child->GetDesiredSize();

            // Account for horizontal scaling
            switch (child->GetHorizontalScaling()) {
                case UIScaling::eNone:
                    break;
                case UIScaling::eFill:
                    desiredSize.x = columnScale.size - (padding.x + padding.z) - adjustedMargin.x;
                    break;
                case UIScaling::eContent:
                    break;
            }

            // Account for vertical scaling
            switch (child->GetVerticalScaling()) {
                case UIScaling::eNone:
                    break;
                case UIScaling::eFill:
                    desiredSize.y = rowScale.size - (padding.y + padding.w) - adjustedMargin.y;
                    break;
                case UIScaling::eContent:
                    break;
            }

            // ...
            child->SetDesiredSize(desiredSize);

            // Determine viewport
            auto cellViewport = Math::Vector4(
                    {pixelTransform.w.x + columnScale.margin, pixelTransform.w.y + rowScale.margin},
                    {pixelTransform.w.x + columnScale.margin + columnScale.size, pixelTransform.w.y + rowScale.margin + rowScale.size}
            );

            // Determine child viewport
            auto childViewport = Math::Vector4(
                    Math::Vector2(Math::Max(cellViewport.x, viewport.x), Math::Max(cellViewport.y, viewport.y)),
                    Math::Vector2(Math::Min(cellViewport.z, viewport.z), Math::Min(cellViewport.w, viewport.w))
            );

            // Final margin
            auto finalMargin = adjustedMargin + adjustedPadding + Math::Vector2(columnScale.margin, rowScale.margin);

            // Final pixel transform
            auto uiTransform = Math::Matrix4::Translate({finalMargin + adjustedPadding, 0}) * pixelTransform;

            // Is the child transform visible?
            child->SetTransform(uiTransform);

            // Child may not inherit the viewport
            switch (child->GetViewportMode()) {
                case UIViewportMode::eNone:
                    child->SetViewport({-1e6, -1e6, 1e6, 1e6});
                    break;
                case UIViewportMode::eKeep:
                    child->SetViewport(GetViewport());
                    break;
                case UIViewportMode::eInherit:
                    child->SetViewport(childViewport);
                    break;
            }
        }
    }
}

void UITableTransformComponent::SetCell(UInt32 row, UInt32 column, IUIElement *element) {
    if (!element) {
        DJINN_ASSERT(false, "Not implemented");
    }

    GetActor()->AssertReference(element);

    auto transform = element->GetTransform();

    transform->SetParentReference(this);
    Bind(element);
    rows[row].cells[column] = element->GetWeakPtr();

    transform->MarkAsDirty(true);
}

void UITableTransformComponent::Configure(UInt32 rowCount, UInt32 columnCount) {
    // Handle rows
    if (rowCount < rows.Size()) {
        auto num = rows.Size() - rowCount;
        for (USize i = 0; i < num; i++) {
            for (auto &&cell : rows[i].cells) {
                if (cell) {
                    cell->Destroy();
                }
            }
        }
    }
    rows.Resize(rowCount);

    // Handle columns
    for (auto &&row : rows) {
        if (columnCount < row.cells.Size()) {
            auto num = row.cells.Size() - columnCount;
            for (USize i = 0; i < num; i++) {
                if (row.cells[i]) {
                    row.cells[i]->Destroy();
                }
            }
        }
        row.cells.Resize(columnCount);
    }

    rowScaling.Resize(rowCount);
    columnScaling.Resize(columnCount);
}

IUITransformComponent *UITableTransformComponent::TraceChild(const Math::Vector2 &point, TransformTraceMode mode) {
    for (auto &&row : rows) {
        for (auto &&child : row.cells) {
            if (child) {
                if (auto ptr = child->GetTransform()->TraceChild(point, mode)) {
                    return ptr;
                }
            }
        }
    }

    if (isTraceFallthrough && mode == TransformTraceMode::eInput) {
        return nullptr;
    }

    return IsPointInside(point) ? this : nullptr;
}

IUITransformComponent *UITableTransformComponent::FindChild(const Core::StringView &name) {
    for (auto &&row : rows) {
        for (auto &&child : row.cells) {
            if (child) {
                if (child->GetName() == name) {
                    return child->GetTransform();
                }

                if (auto ptr = child->GetTransform()->FindChild(name)) {
                    return ptr;
                }
            }
        }
    }

    return nullptr;
}

bool UITableTransformComponent::OnChildDetached(IUITransformComponent *child) {
    InfoLog(this).Write("OnChildDetached not implemented, fuck off");
    return false;
}

void UITableTransformComponent::OnDirty() {
    for (auto &&row : rows) {
        for (auto &&child : row.cells) {
            if (child) {
                child->GetTransform()->MarkAsDirty();
            }
        }
    }
}
