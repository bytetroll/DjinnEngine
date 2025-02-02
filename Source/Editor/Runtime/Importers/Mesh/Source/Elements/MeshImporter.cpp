//
// Created by Dev on 7/11/2018.
//

#include <Editor/Elements/MeshImporter.h>
#include <Base/DefaultLogs.h>
#include <Base/IFileSystem.h>
#include <Base/IFileProxy.h>
#include <Game/IEntitySystem.h>
#include <UI/ElementComponents/IUITransformComponent.h>

DJINN_NS2(Editor);

MeshImporter::MeshImporter(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IControl(registry, outer, host) {

}

void MeshImporter::StaticInitialize() {
    Construct("Runtime/Editor/UI/MeshImporter.uiml"_path);

    GetTransform()->SetScaling(UI::UIScaling::eFill, UI::UIScaling::eFill);
}

void MeshImporter::Created() {
    // Get file system
    if (!registry->GetPipeline()->GetInterface(&fs)) {
        ErrorLog(this).Write("Expected file system to be installed");
        return;
    }
}
