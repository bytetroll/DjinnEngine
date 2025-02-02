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


#include <Editor/Elements/ContentBrowser.h>
#include <Base/DefaultLogs.h>
#include <Base/IFileSystem.h>
#include <Editor/IAssetRegistry.h>
#include <Editor/IIndexedDirectory.h>
#include <Base/IFileProxy.h>
#include <UI/ElementComponents/IUITransformComponent.h>
#include <UI/Elements/UITreeItem.h>
#include <Game/IActorSystem.h>
#include <UI/Elements/UITreeView.h>
#include <Editor/Elements/ContentPreview.h>

DJINN_NS2(Editor);

ContentBrowser::ContentBrowser(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IContentBrowser(registry, outer, host) {

}

void ContentBrowser::StaticInitialize() {
    Construct("Runtime/Editor/UI/ContentBrowser.uiml"_path);

    GetTransform()->SetScaling(UI::UIScaling::eFill, UI::UIScaling::eFill);
}

void ContentBrowser::Created() {
    // Get file system
    if (!registry->GetPipeline()->GetInterface(&fs)) {
        ErrorLog(this).Write("Expected file system to be installed");
        return;
    }

    // Get asset registry
    if (!registry->GetPipeline()->GetInterface(&assetRegistry)) {
        ErrorLog(this).Write("Expected asset registry to be installed");
        return;
    }

    // Populate root
    Populate(nullptr, assetRegistry->GetDirectory());
}

void ContentBrowser::Populate(UI::UITreeItem* insertion, IIndexedDirectory *directory) {
    // Populate directories
    for (auto&& dir : directory->GetDirectories()) {
        UI::UITreeItem* item;
        if (!GetSystem()->CreateActor(GetScene(), &item)) {
            return;
        }

        item->SetText(dir->GetName());

        // When expanded populate this item
        item->BindExpanded(this, [=] {
            Populate(item, dir);
        });

        // When clicked populate all assets
        item->BindClicked(this, [=] {
            PopulateAssets(dir);
        });

        // Add child
        if (insertion)
            insertion->AddChild(item->GetTransform());
        else
            tree->AddChild(item->GetTransform());
    }
}

void ContentBrowser::PopulateAssets(IIndexedDirectory *directory) {
    items->RemoveChilden();

    for (auto&& path : directory->GetAssets()) {
        Core::StringView name;

        // Attempt to get path
        Base::IFileProxy* proxy;
        if (!fs->GetFileProxy(path, &proxy) || !proxy->GetName(path, &name)) {
            continue;
        }

        ContentPreview* preview;
        if (!GetSystem()->CreateActor(GetScene(), &preview)) {
            return;
        }

        preview->SetText(name);

        auto transform = preview->GetTransform();
        transform->SetDesiredSize({80, 80});

        items->AddChild(preview->GetTransform());
    }
}
