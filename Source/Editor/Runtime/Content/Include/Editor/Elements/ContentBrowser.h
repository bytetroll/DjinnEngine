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

#pragma once

#include <UI/UIMLAnnotation.h>
#include <Editor/Elements/IContentBrowser.h>
#include <UI/Elements/UIStack.h>
#include <UI/Elements/UITreeItem.h>
#include <UI/Elements/UITreeView.h>

namespace Djinn::Base {
    class IFileSystem;
}

namespace Djinn::Editor {
    class IIndexedDirectory;
    class IAssetRegistry;

    ClassAttr(UIML::Class("editor-content-browser"))
    class ContentBrowser : public IContentBrowser {
    public:
        DJINN_COM_CLASS();

        ContentBrowser(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;
        void Created() override;

    public:
        Attr(UIML::Bind()) UI::UITreeView* tree;
        Attr(UIML::Bind()) UI::UIStack* items;

    private:
        void Populate(UI::UITreeItem* insertion, IIndexedDirectory* directory);
        void PopulateAssets(IIndexedDirectory* directory);

    private:
        IAssetRegistry* assetRegistry;
        Base::IFileSystem* fs;
    };
}
