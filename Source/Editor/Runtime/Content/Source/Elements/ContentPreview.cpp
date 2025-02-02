//
// Created by Dev on 7/11/2018.
//

#include <Editor/Elements/ContentPreview.h>
#include <Base/DefaultLogs.h>
#include <UI/Elements/UILabel.h>

DJINN_NS2(Editor);

ContentPreview::ContentPreview(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IControl(registry, outer, host) {

}

void ContentPreview::StaticInitialize() {
    Construct("Runtime/Editor/UI/ContentPreview.uiml"_path);
}

void ContentPreview::Created() {

}

void ContentPreview::SetText(const Core::StringView &text) {
    FindChild<UI::UILabel>("name")->SetText(text);
}
