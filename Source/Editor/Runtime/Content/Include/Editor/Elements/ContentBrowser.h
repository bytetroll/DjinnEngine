//
// Created by Dev on 7/11/2018.
//
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
